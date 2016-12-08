#ifndef ENTITYCOMMAND_H
#define ENTITYCOMMAND_H

#include "Command.h"

#include "../ECS/EntityManager.h"
#include "../ECS/ComponentRegistry.h"

namespace engine {
    namespace IO {
        class EntityCommand : public Command {
            
            using EntityManager = engine::ECS::EntityManager;
            using ComponentRegistry = engine::ECS::ComponentRegistry;
            using entityId_t = engine::ECS::entityId_t;
            using componentId_t = engine::ECS::componentId_t;
        
        protected:
            static const std::string SUBCMD_LIST;
            static const std::string SUBCMD_GET;
            static const std::string SUBCMD_SET;
            
            EntityManager& em;
            
            void printHelp(std::ostream& out) const;
            
        public:
            EntityCommand(EntityManager& em) : em(em) {}
            
            std::string getName() const override {
                return "entity";
            }
            
            int main(const vector<std::string>& args, std::istream& stdin, std::ostream& stdout, std::ostream& stderr) override;
            
            int list();
            int get(entityId_t entity, componentId_t component = SIZE_MAX);
            int set(entityId_t entity, componentId_t component, const std::string& json);
            
            
            inline int get(std::string entity, std::string component) {
                throw 42;
            }
            inline int get(entityId_t entity, std::string component) {
                return this->get(entity, ComponentRegistry::getComponentTypeId(component));
            }
            inline int get(std::string entity, componentId_t component) {
                throw 42;
            }
            
            int set(std::string entity, std::string component, const std::string& json) {
                throw 42;
            }
            int set(std::string entity, componentId_t component, const std::string& json) {
                throw 42;
            }
            int set(entityId_t entity, std::string component, const std::string& json) {
                return this->set(entity, ComponentRegistry::getComponentTypeId(component), json);
            }
        };
    }
}

#endif /* ENTITYCOMMAND_H */

