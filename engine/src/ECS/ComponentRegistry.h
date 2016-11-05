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
        };
    }
}

#endif /* COMPONENTREGISTRY_H */

