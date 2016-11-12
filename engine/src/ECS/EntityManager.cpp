#include "EntityManager.h"

#include "Entity.h"

namespace engine {
    namespace ECS {
        EntityManager::EntityManager() {
            components.set_empty_key(SIZE_MAX);
        }

        EntityManager::~EntityManager() {
        }
        
        Entity EntityManager::createEntity(const std::string& name) {
            return Entity(*this, name);
        }
        
        void EntityManager::addComponent(const Entity& e, const shared_ptr<Component>& comp) {
            this->components[comp->getComponentId()].push_back(comp);
        }
        
        EntityManager::ComponentIterator EntityManager::getComponentIterator(const std::initializer_list<componentId_t>& componentTypes) {
            return ComponentIterator(*this, componentTypes);
        }
        
        
        EntityManager::ComponentIterator::ComponentIterator(EntityManager& em,
                const std::initializer_list<componentId_t>& componentTypes)
                : em(em), componentTypes(componentTypes), components(componentTypes.size()) {
        }

        EntityManager::ComponentIterator::ComponentIterator(const ComponentIterator& ci)
                : em(ci.em), componentTypes(ci.componentTypes), components(ci.components) {
        }

        EntityManager::ComponentIterator::ComponentIterator(ComponentIterator&& ci)
                : em(ci.em), componentTypes(std::move(ci.componentTypes)), components(std::move(ci.components)) {}

        EntityManager::ComponentIterator::~ComponentIterator() {}
        
        void EntityManager::ComponentIterator::swap(ComponentIterator& ci) {
            std::swap(this->components, ci.components);
            std::swap(this->componentTypes, ci.componentTypes);
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
            return this->components[index];
        }
    }
}

