#ifndef COMPONENTREGISTRY_H
#define COMPONENTREGISTRY_H

namespace engine {
    namespace ECS {
        class ComponentRegistry {
        private:
            static int nextId;

        public:
            static int getNextId() {
                return ComponentRegistry::nextId++;
            }
        };
    }
}

#endif /* COMPONENTREGISTRY_H */

