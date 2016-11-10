#ifndef SYSTEMREGISTRY_H
#define SYSTEMREGISTRY_H

#include "System.h"

namespace engine {
    namespace ECS {
        class SystemRegistry {
        private:
            static systemId_t nextId;

        public:
            static systemId_t getNextId() {
                return SystemRegistry::nextId++;
            }
        };
    }
}

#endif /* SYSTEMREGISTRY_H */

