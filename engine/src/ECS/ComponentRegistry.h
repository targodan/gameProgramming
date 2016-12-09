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
            struct CompInfo {
                CompInfo() : name(""), instantiator(nullptr) {}
                CompInfo(std::string name, ComponentInstantiator* instantiator) : name(name), instantiator(instantiator) {}
                CompInfo(const CompInfo& other) : name(other.name), instantiator(other.instantiator) {}
                CompInfo(CompInfo&& other) { this->swap(other); }
                CompInfo& operator=(const CompInfo& right) { this->name = right.name; this->instantiator = right.instantiator; return *this; }
                void swap(CompInfo& right) { std::swap(this->name, right.name); std::swap(this->instantiator, right.instantiator); }
                std::string name;
                ComponentInstantiator* instantiator;
            };
            
            static componentId_t nextId;
            static std::unique_ptr<Map<componentId_t, CompInfo>> registeredComponents;
            static std::unique_ptr<Map<std::string, componentId_t>> componentNames;

        public:
            static componentId_t getNextId() {
                return ComponentRegistry::nextId++;
            }
            
            static void registerComponent(componentId_t id, std::string name, ComponentInstantiator* ci);
            static Component* makeComponentOfType(componentId_t id);
            static Component* makeComponentOfType(std::string name);
            static componentId_t getComponentTypeId(std::string name);
            static std::string getComponentTypeName(componentId_t id);
        };
    }
}

#endif /* COMPONENTREGISTRY_H */

