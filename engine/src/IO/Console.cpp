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
        
        Console::Console(size_t cmdHistorySize) : isCommandRunning(false), cmdHistory(cmdHistorySize) {
            this->linebuffer.reserve(128);
            this->commands.set_empty_key("");
        }
        
        Console::~Console() {
            for(auto& elem : this->commands) {
                delete elem.second;
            }
            this->commands.clear();
        }
        
        void Console::receiveKeypress(char key) {
            if(key == '\n') /* Enter */ {
                auto args = this->parseLine(this->linebuffer);
                this->linebuffer = "";
                this->executeCommand(args);
            } else {
                this->linebuffer.append(&key, 1);
            }
        }
        
        vector<std::string> Console::parseLine(const std::string line) const {
            vector<std::string> ret;
            size_t i = 0;
            while(i < line.size()) {
                std::string token = this->getNextToken(line, i);
                if(token != "") {
                    ret.push_back(token);
                }
            }
            return ret;
        }
        
        std::string Console::getNextToken(const std::string line, size_t& io_pos) const {
            std::string ret;
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
                            ret.append(&c, 1);
                            break;
                        case 't':
                            tmp = '\t';
                            ret.append(&tmp, 1);
                            break;
                        case 'n':
                            tmp = '\n';
                            ret.append(&tmp, 1);
                            break;
                        default:
                            tmp = '\\';
                            ret.append(&tmp, 1);
                            ret.append(&c, 1);
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
                                ret.append(&c, 1);
                            } else {
                                stop = true;
                            }
                            break;
                        default:
                            ret.append(&c, 1);
                    }
                }
                
                if(io_pos >= line.size()) {
                    stop = true;
                }
            }
            return ret;
        }
        
        void Console::executeCommand(const vector<std::string>& args) {
            std::async(std::launch::deferred, [=]() { this->echo(this->ps1); });
            if(args.size() > 0) {
                auto command = this->commands.find(args[0]);
                if(command == this->commands.end()) {
                    *this << "Invalid command \"" << args[0] << "\"" << Console::endl;
                    return;
                }
                this->isCommandRunning = true;
                std::promise<int> promise;
                this->runningCommandFuture = promise.get_future();
                this->runningCommandThread = std::thread([=](std::promise<int> promise) {
                    try {
                        promise.set_value(command->second->main(args, this->stdin, this->stdout, this->stderr));
                    } catch(...) {
                        promise.set_exception(std::current_exception());
                    }
                    this->isCommandRunning = false;
                }, std::move(promise));
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
                while(multiplier > 1 && this->outputBuffer.size() - (multiplier * overflow) >= minRemainingText) {
                    --multiplier;
                }
                this->outputBuffer.erase(0, multiplier * overflow);
            }
        }
    }
}
