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
        
        
        EntityManager::ComponentIterator::ComponentIterator(EntityManager& em,
                std::initializer_list<int> componentTypes) : em(em) {
            this->numTypes = componentTypes.size();
            this->componentTypes = new int[this->numTypes];
            this->components = new shared_ptr<Component>[this->numTypes];
            std::copy(componentTypes.begin(), componentTypes.end(), this->componentTypes);
        }

        EntityManager::ComponentIterator::ComponentIterator(const ComponentIterator& ci) : em(ci.em) {
            this->componentTypes = new int[ci.numTypes];
            this->components = new shared_ptr<Component>[this->numTypes];
            std::copy(ci.componentTypes, ci.componentTypes + sizeof(int) * ci.numTypes, this->componentTypes);
        }

        EntityManager::ComponentIterator::ComponentIterator(ComponentIterator&& ci) : em(ci.em) {
            this->componentTypes = ci.componentTypes;
            this->components = ci.components;
        }

        EntityManager::ComponentIterator::~ComponentIterator() {
            if(this->componentTypes != nullptr) {
                delete[] this->componentTypes;
            }
            if(this->components != nullptr) {
                delete[] this->components;
            }
        }

        EntityManager::ComponentIterator& EntityManager::ComponentIterator::operator++() {
            // Do actual increment
            return *this;
        }

        EntityManager::ComponentIterator EntityManager::ComponentIterator::operator++(int) {
            ComponentIterator result(*this);
            this->operator++();
            return result;
        }

        shared_ptr<Component> EntityManager::ComponentIterator::operator*() {
            return this->operator[](0);
        }

        shared_ptr<Component> EntityManager::ComponentIterator::operator->() {
            return this->operator[](0);
        }

        shared_ptr<Component> EntityManager::ComponentIterator::operator[](std::size_t index) {
#ifdef DEBUG
            if(index >= this->numTypes) {
                throw WTFException("You did not request enough types.");
            }
#endif
            return this->components[index];
        }
    }
}

