#ifndef COMPONENTREGISTRY_H
#define COMPONENTREGISTRY_H

#include <stdlib.h>

#include "Component.h"

namespace engine {
    namespace ECS {
        class ComponentRegistry {
        private:
            static componentId_t nextId;

        public:
            static componentId_t getNextId() {
                return ComponentRegistry::nextId++;
            }
            
            static Component* makeComponentOfType(componentId_t i);
        };
    }
}

#endif /* COMPONENTREGISTRY_H */

