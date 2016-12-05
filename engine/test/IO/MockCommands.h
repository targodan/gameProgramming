#ifndef MOCKCOMMAND_H
#define MOCKCOMMAND_H

#include "IO/Command.h"

class MockCommand_ArgsTest : public engine::IO::Command {
public:
    engine::util::vector<std::string> receivedArgs;
    
    std::string getName() const override {
        return "args";
    }
    
    int main(const engine::util::vector<std::string>& args, std::istream& stdin, std::ostream& stdout, std::ostream& stderr) override {
        this->receivedArgs = args;
        return 42;
    }
};

class MockCommand_OutTest : public engine::IO::Command {
public:
    std::string getName() const override {
        return "echo";
    }
    
    int main(const engine::util::vector<std::string>& args, std::istream& stdin, std::ostream& stdout, std::ostream& stderr) override {
        for(size_t i = 1; i < args.size(); ++i) {
            auto& arg = args[i];
            stdout << arg;
            if(i+1 < args.size()) {
                stdout << " ";
            }
        }
        stdout << engine::IO::Console::endl;
        return 666;
    }
};

#endif /* MOCKCOMMAND_H */

