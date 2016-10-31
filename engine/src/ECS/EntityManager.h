#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <vector>
#include <memory>

#include "Component.h"
#include "Entity.h"

using std::vector;
using std::shared_ptr;

namespace engine {
    namespace ECS {
        class EntityManager {
        private:
            vector<shared_ptr<Component>> components;

        public:
            EntityManager();
            EntityManager(const EntityManager& orig);
            virtual ~EntityManager();
            
            Entity addEntity();
        };
    }
}

#endif /* ENTITYMANAGER_H */

