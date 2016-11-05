#ifndef SYSTEMREGISTRY_H
#define SYSTEMREGISTRY_H

#include "System.h"

namespace engine {
    namespace ECS {
        class SystemRegistry {
        private:
            static systemTypeId nextId;

        public:
            static systemTypeId getNextId() {
                return SystemRegistry::nextId++;
            }
        };
    }
}

#endif /* SYSTEMREGISTRY_H */

