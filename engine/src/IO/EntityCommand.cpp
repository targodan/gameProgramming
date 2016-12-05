#include "EntityCommand.h"
#include "Console.h"

namespace engine {
    namespace IO {
        void EntityCommand::printHelp(std::ostream& out) const {
            out     <<  "Usage: entity <command> [args...]" << Console::endl
                    <<  Console::endl
                    <<  "Available commands:" << Console::endl
                    <<  "   list                            Lists all registered entities." << Console::endl
                    <<  "   get <entity> [component]        Shows all components, or only the specified [component]," << Console::endl
                    <<  "                                   of the given <entity>." << Console::endl
                    <<  "   get <entity> <component> <json> Sets the <component> of the <entity> to the values as" << Console::endl
                    <<  "                                   specified by the <json> representation." << Console::endl
                    <<  Console::endl
                    <<  "Arguments:" << Console::endl
                    <<  "   <entity>    The name or ID of an entity." << Console::endl
                    <<  "   <component> The name or ID of a component type." << Console::endl
                    <<  "   <json>      A string in JSON syntax. NOTE: Remember to (double) escape any quotes" << Console::endl
                    <<  "               necessary. (\"{\\\"field\\\": \\\"someStringWith \\\\\"quotes\\\\\"\\\"}\")" << Console::endl;
        }
        
        int EntityCommand::main(const vector<std::string>& args, std::istream& stdin, std::ostream& stdout, std::ostream& stderr) {
            if(args.size() < 2) {
                this->printHelp(stderr);
                return 1;
            }
            return 0;
        }
    }
}