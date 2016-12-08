#include "EntityCommand.h"
#include "Console.h"

namespace engine {
    namespace IO {
        const std::string EntityCommand::SUBCMD_LIST = "list";
        const std::string EntityCommand::SUBCMD_GET = "get";
        const std::string EntityCommand::SUBCMD_SET = "set";

        void EntityCommand::printHelp(std::ostream& out) const {
            out     <<  "Usage: entity <command> [args...]" << Console::endl
                    <<  Console::endl
                    <<  "Available commands:" << Console::endl
                    <<  "   list                            Lists all registered entities." << Console::endl
                    <<  "   get <entity> [component]        Shows all components, or only the specified [component]," << Console::endl
                    <<  "                                   of the given <entity>." << Console::endl
                    <<  "   set <entity> <component> <json> Sets the <component> of the <entity> to the values as" << Console::endl
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
            if(args[1] == "--help" || args[1] == "-h") {
                this->printHelp(stderr);
                return 0;
            }
            
            if(args[1] == EntityCommand::SUBCMD_LIST) {
                if(args.size() > 2) {
                    stderr << "Too many arguments." << Console::endl
                            << "Usage: entity list" << Console::endl;
                    return 1;
                }
                return this->list();
            } else if(args[1] == EntityCommand::SUBCMD_GET) {
                if(args.size() < 3) {
                    stderr << "Not enough arguments." << Console::endl
                            << "Usage: entity get <entity> [component]" << Console::endl;
                    return 1;
                }
                if(args.size() > 4) {
                    stderr << "Too many arguments." << Console::endl
                            << "Usage: entity get <entity> [component]" << Console::endl;
                    return 1;
                }
                std::string entityString = "";
                entityId_t entityId = SIZE_MAX;
                try {
                    entityId = this->strTo<entityId_t>(args[2]);
                } catch(...) { /* This is fine. */ }
                
                componentId_t componentId = SIZE_MAX;
                std::string componentString = "";
                if(args.size() == 4) {
                    try {
                        componentId = this->strTo<componentId_t>(args[3]);
                    } catch(...) { /* This is fine. */ }
                }

                if(entityId != SIZE_MAX) {
                    if(componentId != SIZE_MAX || args.size() < 4) {
                        return this->get(entityId, componentId);
                    } else {
                        return this->get(entityId, componentString);
                    }
                } else {
                    if(componentId != SIZE_MAX || args.size() < 4) {
                        return this->get(entityString, componentId);
                    } else {
                        return this->get(entityString, componentString);
                    }
                }
            } else if(args[1] == EntityCommand::SUBCMD_SET) {
                if(args.size() < 5) {
                    stderr << "Not enough arguments." << Console::endl
                            << "Usage: entity set <entity> <component> <json>" << Console::endl;
                    return 1;
                }
                if(args.size() > 5) {
                    stderr << "Too many arguments." << Console::endl
                            << "Usage: entity set <entity> <component> <json>" << Console::endl;
                    return 1;
                }
                std::string entityString = "";
                entityId_t entityId = SIZE_MAX;
                try {
                    entityId = this->strTo<entityId_t>(args[2]);
                } catch(...) { /* This is fine. */ }
                
                componentId_t componentId = SIZE_MAX;
                std::string componentString = "";
                try {
                    componentId = this->strTo<componentId_t>(args[3]);
                } catch(...) { /* This is fine. */ }
                
                std::string json = args[4];

                if(entityId != SIZE_MAX) {
                    if(componentId != SIZE_MAX) {
                        return this->set(entityId, componentId, json);
                    } else {
                        return this->set(entityId, componentString, json);
                    }
                } else {
                    if(componentId != SIZE_MAX) {
                        return this->set(entityString, componentId, json);
                    } else {
                        return this->set(entityString, componentString, json);
                    }
                }
            } else {
                this->printHelp(stderr);
                return 2;
            }
        }
        
        int EntityCommand::list() {
            return 666;
        }
        
        int EntityCommand::get(entityId_t entity, componentId_t component /*= SIZE_MAX*/) {
            return 666;
        }
        
        int EntityCommand::set(entityId_t entity, componentId_t component, const std::string& json) {
            return 666;
        }
        
    }
}