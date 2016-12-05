#ifndef ENTITYCOMMAND_H
#define ENTITYCOMMAND_H

#include "Command.h"

namespace engine {
    namespace IO {
        class EntityCommand : public Command {
        protected:
            void printHelp(std::ostream& out) const;
            
        public:
            std::string getName() override {
                return "entity";
            }
            
            int main(const vector<std::string>& args, std::istream& stdin, std::ostream& stdout, std::ostream& stderr) const override;
        };
    }
}

#endif /* ENTITYCOMMAND_H */

