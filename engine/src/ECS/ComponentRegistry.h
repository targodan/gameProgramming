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
            static componentId_t nextId;
            static std::unique_ptr<Map<componentId_t, ComponentInstantiator*>> instantiators;

        public:
            static componentId_t getNextId() {
                return ComponentRegistry::nextId++;
            }
            
            static void registerInstantiator(componentId_t id, ComponentInstantiator* ci);
            static Component* makeComponentOfType(componentId_t id);
        };
    }
}

#endif /* COMPONENTREGISTRY_H */

