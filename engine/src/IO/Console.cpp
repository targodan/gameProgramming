#include "Console.h"

#include <algorithm>
#include <future>

namespace engine {
    namespace IO {
        template<>
        Console& operator<<(Console& c, const std::string& msg) {
            c.echo(msg);
            return c;
        }
        
        const std::string Console::endl = "\n";
        
        Console::Console(size_t cmdHistorySize, size_t outputBufferMaxSize) : isCommandRunning(false), lastExitValue(0), ps1("$> "), cmdHistory(cmdHistorySize), outputBufferMaxSize(outputBufferMaxSize) {
            this->commands.set_empty_key("");
            this->printPS1();
        }
        
        Console::~Console() {
            for(auto& elem : this->commands) {
                delete elem.second;
            }
            this->commands.clear();
        }
        
        void Console::receiveKeypress(char key) {
            if(!this->isCommandRunning) {
                if(key == '\n') /* Enter */ {
                    this->executeCommandFromLinebuffer();
                } else {
                    this->linebuffer << key;
                }
            } else {
                this->stdin << key;
            }
            
            // TODO: Handle Ctrl-C as kill command.
            // TODO: Handle special keys like arrows, del, backspace etc.
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
            auto args = this->parseLine(this->linebuffer.str());
            this->echo(this->linebuffer.str());
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
        
        int Console::waitForCommandExit() {
            this->retreiveExitCode();
            return this->lastExitValue;
        }
        
        void Console::printPS1() {
            this->echo(this->ps1);
        }
    }
}
