#include "Console.h"

#include <algorithm>
#include <future>

#include "Keys.h"

namespace engine {
    namespace IO {
        template<>
        Console& operator<<(Console& c, const std::string& msg) {
            c.echo(msg);
            return c;
        }
        
        const std::string Console::endl = "\n";
        
        Console::Console(size_t cmdHistorySize, size_t outputBufferMaxSize)
            : isCommandRunning(false), lastExitValue(0), ps1("$> "),
                cmdHistory(cmdHistorySize), cmdHistorySize(cmdHistorySize), 
                outputBufferMaxSize(outputBufferMaxSize) {
            this->commands.set_empty_key("");
            this->printPS1();
            this->resetHistoryIndex();
            this->linebuffer.setInsertMode(true);
        }
        
        Console::~Console() {
            for(auto& elem : this->commands) {
                delete elem.second;
            }
            this->commands.clear();
        }
        
        void Console::historyUp() {
            if(this->cmdHistoryIndex+1 >= this->cmdHistorySize) {
                return;
            }
            if(this->cmdHistoryIndex == SIZE_MAX) {
                // Not in history yet
                // Add current line (it will be removed by this->updateHistoryOnExecute)
                this->addToHistory(this->linebuffer.str());
                // Set index to 1 (0 is the current linebuffer)
                this->cmdHistoryIndex = 1;
            } else {
                // Already showing history => nothing special to do.
                ++this->cmdHistoryIndex;
            }
            
            // Load history to linebuffer
            this->linebuffer.str(this->cmdHistory[this->cmdHistoryIndex]);
        }
        
        void Console::historyDown() {
            if(this->cmdHistoryIndex == SIZE_MAX || this->cmdHistoryIndex == 0) {
                return;
            }
            --this->cmdHistoryIndex;
            
            // Load history to linebuffer
            this->linebuffer.str(this->cmdHistory[this->cmdHistoryIndex]);
        }
        
        void Console::updateHistoryOnExecute() {
            if(this->cmdHistoryIndex != SIZE_MAX) {
                // History has been used => we need to remove the temporary line 0
                this->cmdHistory.pop_front();
            }
            this->addToHistory(this->linebuffer.str());
            this->resetHistoryIndex();
        }
        
        void Console::resetHistoryIndex() {
            this->cmdHistoryIndex = SIZE_MAX;
        }
        
        void Console::addToHistory(const std::string& line) {
            if(this->cmdHistory.size()+1 >= this->cmdHistorySize) {
                this->cmdHistory.pop_back();
            }
            this->cmdHistory.push_front(line);
        }
        
        void Console::receiveKeypress(char key) {
            std::ios::pos_type origPos;
            if(!this->isCommandRunning) {
                switch(key) {
                    case Keys::ENTER:
                        this->executeCommandFromLinebuffer();
                        break;
                    case Keys::ARROW_LEFT:
                        if(this->linebuffer.tellp() > 0) {
                            this->linebuffer.seekp(-1, std::ios::cur);
                        }
                        break;
                    case Keys::ARROW_RIGHT:
                        origPos = this->linebuffer.tellp();
                        this->linebuffer.seekp(0, std::ios::end);
                        // origPos is not the end?
                        if(origPos != this->linebuffer.tellp()) {
                            this->linebuffer.seekp(origPos);
                            this->linebuffer.seekp(1, std::ios::cur);
                        }
                        break;
                    case Keys::ARROW_UP:
                        this->historyUp();
                        break;
                    case Keys::ARROW_DOWN:
                        this->historyDown();
                        break;
                    case Keys::BACKSPACE:
                        origPos = this->linebuffer.tellp();
                        this->linebuffer.seekp(0, std::ios::beg);
                        // origPos is not the beg?
                        if(origPos != this->linebuffer.tellp()) {
                            this->linebuffer.seekp(origPos);
                            this->linebuffer.removePreviousChar();
                        }
                        break;
                    case Keys::DEL:
                        origPos = this->linebuffer.tellp();
                        this->linebuffer.seekp(0, std::ios::end);
                        // origPos is not the end?
                        if(origPos != this->linebuffer.tellp()) {
                            this->linebuffer.seekp(origPos);
                            this->linebuffer.removeNextChar();
                        }
                        break;
                    case Keys::CTRL_C:
                        this->killRunningCommand();
                        break;
                    default:
                        this->linebuffer << key;
                }
            } else {
                this->stdin << key;
            }
        }
        
        vector<std::string> Console::parseLine(const std::string line) const {
            vector<std::string> tokens;
            size_t i = 0;
            while(i < line.size()) {
                std::string token = this->getNextToken(line, i);
                if(token != "") {
                    tokens.push_back(token);
                }
            }
            return tokens;
        }
        
        std::string Console::getNextToken(const std::string line, size_t& io_pos) const {
            std::string token;
            bool stop = false;
            bool escape = false;
            bool quotes = false;
            while(!stop) {
                char c = line[io_pos++];
                char tmp = c;
                
                if(escape) {
                    switch(c) {
                        case '\\':
                        case '"':
                        case ' ':
                            token.append(&c, 1);
                            break;
                        case 't':
                            tmp = '\t';
                            token.append(&tmp, 1);
                            break;
                        case 'n':
                            tmp = '\n';
                            token.append(&tmp, 1);
                            break;
                        default:
                            tmp = '\\';
                            token.append(&tmp, 1);
                            token.append(&c, 1);
                    }
                    escape = false;
                } else {
                    switch(c) {
                        case '\\':
                            escape = true;
                            break;
                        case '"':
                            quotes = !quotes;
                            break;
                        case ' ':
                            if(quotes) {
                                token.append(&c, 1);
                            } else {
                                stop = true;
                            }
                            break;
                        default:
                            token.append(&c, 1);
                    }
                }
                
                if(io_pos >= line.size()) {
                    stop = true;
                }
            }
            return token;
        }
        
        void Console::executeCommandFromLinebuffer() {
            std::string line = this->linebuffer.str();
            this->updateHistoryOnExecute();
            auto args = this->parseLine(line);
            this->echo(line);
            this->echo(Console::endl);
            this->linebuffer.str("");
            this->executeCommand(args);
        }
        
        void Console::executeCommand(const vector<std::string>& args) {
            this->retreiveExitCode();
            if(args.size() > 0) {
                const auto& command = this->commands.find(args[0]);
                if(command == this->commands.end()) {
                    *this << "Invalid command \"" << args[0] << "\"" << Console::endl;
                    return;
                }
                this->isCommandRunning = true;
                std::promise<int> promise;
                this->runningCommandFuture = promise.get_future();
                this->runningCommandThread = std::thread([this](std::promise<int> promise, Command* command, vector<std::string> args) {
                    try {
                        promise.set_value(command->main(args, this->stdin, this->stdout, this->stderr));
                    } catch(...) {
                        promise.set_exception(std::current_exception());
                    }
                    this->isCommandRunning = false;
                }, std::move(promise), command->second, args);
            }
        }
        
        void Console::echo(const std::string& msg) {
            this->outputBuffer.append(msg);
        }
        
        void Console::shrinkOutputBufferIfNeeded() {
            if(this->outputBuffer.size() > this->outputBufferMaxSize) {
                // Remove more than necessary, so we don't need to do this every time,
                // but keep at least minRemainingText in the buffer.
                size_t overflow = this->outputBuffer.size() - this->outputBufferMaxSize;
                size_t multiplier = 6;
                size_t minRemainingText = this->outputBufferMaxSize / 2;
                while(multiplier > 1 && this->outputBuffer.size() - (multiplier * overflow) < minRemainingText) {
                    --multiplier;
                }
                this->outputBuffer.erase(0, multiplier * overflow);
            }
        }
        
        void Console::updateOutputBuffer() {
            this->echo(this->stdout.toStringThenEmpty());
            this->echo(this->stderr.toStringThenEmpty());
            this->shrinkOutputBufferIfNeeded();
        }
        
        void Console::tick() {
            if(!this->isCommandRunning) {
                this->retreiveExitCode();
            }
            this->updateOutputBuffer();
        }
        
        std::string Console::getOutput() {
            this->updateOutputBuffer();
            std::string output(this->outputBuffer);
            output.append(this->linebuffer.str());
            return output;
        }
        
        void Console::retreiveExitCode() {
            if(this->runningCommandFuture.valid()) {
                this->lastExitValue = this->runningCommandFuture.get();
                this->runningCommandFuture = std::future<int>();
                if(this->runningCommandThread.joinable()) {
                    this->runningCommandThread.join();
                }
                this->updateOutputBuffer();
                this->printPS1();
            }
        }
        
        void Console::killRunningCommand() {
            if(!this->isCommandRunning) {
                return;
            }
            this->runningCommandThread = std::thread();
            this->isCommandRunning = false;
            this->runningCommandFuture = std::future<int>();
            this->lastExitValue = -1;
            this->updateOutputBuffer();
            *this << "Process terminated." << Console::endl;
            this->printPS1();
        }
        
        int Console::waitForCommandExit() {
            this->retreiveExitCode();
            return this->lastExitValue;
        }
        
        void Console::printPS1() {
            this->echo(this->ps1);
        }
    }
}
