#ifndef CONSOLE_H
#define CONSOLE_H

#include <iostream>
#include <string>
#include <future>
#include <atomic>
#include <deque>

#include "../CollisionException.h"
#include "../util/Map.h"
#include "../util/Array.h"
#include "../util/vector.h"
#include "../util/stringstream.h"

#include "Command.h"

namespace engine {
    namespace IO {
        using engine::util::Map;
        using engine::util::Array;
        using engine::util::vector;
        using engine::util::stringstreamTS;
        using engine::util::stringstreamInsertable;
        
        class Console {
        protected:
            Map<std::string, Command*> commands;
            bool isCommandRunning;
            int lastExitValue;
            std::future<int> runningCommandFuture;
            std::thread runningCommandThread;
            stringstreamTS stdin;
            stringstreamTS stdout;
            stringstreamTS stderr;
            
            stringstreamInsertable linebuffer;
            
            std::string ps1;
            
            std::deque<std::string> cmdHistory;
            size_t cmdHistoryIndex;
            size_t cmdHistorySize;
            
            std::string outputBuffer;
            size_t outputBufferMaxSize;
            
            vector<std::string> parseLine(const std::string line) const;
            std::string getNextToken(const std::string line, size_t& io_pos) const;
            void executeCommandFromLinebuffer();
            void addToHistory(const std::string& line);
            void executeCommand(const vector<std::string>& args);
            void historyUp();
            void historyDown();
            void resetHistoryIndex();
            void updateHistoryOnExecute();
            void killRunningCommand();
            
            void retreiveExitCode();
            void updateOutputBuffer();
            void printPS1();
            
            void echo(const std::string& msg);
            void shrinkOutputBufferIfNeeded();
            
            template<class T>
            friend Console& operator<<(Console& c, const T& msg);
            
        public:
            Console(size_t cmdHistorySize, size_t outputBufferMaxSize);
            Console(Console& orig) = delete;
            ~Console();
            
            static const std::string endl;
            
            template<class CommandT, typename... Args>
            CommandT& registerCommand(Args... args) {
                auto cmd = new CommandT(args...);
                if(this->commands.find(cmd->getName()) != this->commands.end()) {
                    throw engine::CollisionException("A command with the name \"%s\" has already been registered!", cmd->getName().c_str());
                }
                this->commands[cmd->getName()] = cmd;
                return *cmd;
            }
            
            void tick();
            void receiveKeypress(char key);
            int waitForCommandExit();
            
            std::string getOutput();
        };

        template<class T>
        Console& operator<<(Console& c, const T& msg) {
            std::stringstream ss;
            ss << msg;
            c.echo(ss.str());
            return c;
        }
    }
}

#endif /* CONSOLE_H */

