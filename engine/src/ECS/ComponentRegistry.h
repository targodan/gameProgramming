#ifndef COMPONENTREGISTRY_H
#define COMPONENTREGISTRY_H

#include <google/protobuf/message.h>

#include <stdlib.h>

#include "../util/Map.h"
#include <memory>

#include "Component.h"

namespace engine {
    namespace ECS {
        using engine::util::Map;
        
        class ComponentInstantiator;
        class ProtobufMessageInstantiator;
        
        class ComponentRegistry {
        private:
            struct ComponentInfo {
                ComponentInfo() : name(""), instantiator(nullptr) {}
                ComponentInfo(std::string name, ComponentInstantiator* instantiator) : name(name), instantiator(instantiator) {}
                ComponentInfo(const ComponentInfo& other) : name(other.name), instantiator(other.instantiator) {}
                ComponentInfo(ComponentInfo&& other) { this->swap(other); }
                ComponentInfo& operator=(const ComponentInfo& right) { this->name = right.name; this->instantiator = right.instantiator; return *this; }
                void swap(ComponentInfo& right) { std::swap(this->name, right.name); std::swap(this->instantiator, right.instantiator); }
                std::string name;
                ComponentInstantiator* instantiator;
            };
            
            static componentId_t nextId;
            static std::unique_ptr<Map<componentId_t, ComponentInfo>> registeredComponents;
            static std::unique_ptr<Map<std::string, componentId_t>> componentNames;

        public:
            static componentId_t getNextId() {
                return ComponentRegistry::nextId++;
            }
            
            static void registerComponent(componentId_t id, std::string name, ComponentInstantiator* ci);
            static Component* constructComponentOfType(componentId_t id);
            static Component* constructComponentOfType(std::string name);
            static componentId_t getComponentTypeId(std::string name);
            static std::string getComponentTypeName(componentId_t id);
        };
    }
}

#endif /* COMPONENTREGISTRY_H */

