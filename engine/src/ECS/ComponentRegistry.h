#ifndef COMPONENTREGISTRY_H
#define COMPONENTREGISTRY_H

#include <stdlib.h>

#include "Component.h"

namespace engine {
    namespace ECS {
        class ComponentRegistry {
        private:
            static compTypeId nextId;

        public:
            static compTypeId getNextId() {
                return ComponentRegistry::nextId++;
            }
        };
    }
}

#endif /* COMPONENTREGISTRY_H */

