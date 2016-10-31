#include "EntityManager.h"

namespace engine {
    namespace ECS {
        EntityManager::EntityManager() {
        }

        EntityManager::EntityManager(const EntityManager& orig) {
        }

        EntityManager::~EntityManager() {
        }
        
        Entity EntityManager::addEntity(const std::string& name) {
            Entity e(name);
            return e;
        }
        
        EntityManager::ComponentIterator EntityManager::getComponentIterator(std::initializer_list<int> componentTypes) {
            return ComponentIterator(*this, componentTypes);
        }
    }
}

