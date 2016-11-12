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
            auto& vec = this->components[comp->getComponentId()];
            vec.push_back(comp);
            std::sort(vec.begin(), vec.end(),
                    [](const auto& l, const auto& r) {return l->getEntityId() < r->getEntityId();});
        }
            
        EntityManager::ComponentIterator EntityManager::begin(const std::initializer_list<componentId_t>& componentTypes) {
            return ComponentIterator(this, componentTypes);
        }
        
        EntityManager::ComponentIterator EntityManager::end(const std::initializer_list<componentId_t>& componentTypes) {
            auto it = ComponentIterator(this, componentTypes);
            it.setToEnd();
            return it;
        }
        
        
        EntityManager::ComponentIterator::ComponentIterator(EntityManager* em,
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
            std::swap(this->em, ci.em);
            std::swap(this->components, ci.components);
            std::swap(this->componentTypes, ci.componentTypes);
        }
        
        void EntityManager::ComponentIterator::setToEnd() {
            for(size_t i = 0; i < this->componentTypes.size(); ++i) {
                this->components[i] = this->em->components[this->componentTypes[this->components[0]]].size();
            }
        }

        EntityManager::ComponentIterator& EntityManager::ComponentIterator::operator++() {
            if(this->components[0] >= this->em->components[this->componentTypes[this->components[0]]].size()) {
                // Allready at end.
                return *this;
            }
            do {
                ++this->components[0];
                entityId_t nextId = this->operator[](0)->getEntityId();
                bool found = true;
                for(size_t t = 1; t < this->componentTypes.size(); ++t) {
                    auto& compList = this->em->components[this->componentTypes[t]];
                    do {
                        ++this->components[t];
                    } while(this->components[t] < compList.size()
                            && compList[this->components[t]]->getEntityId() <= nextId);
                    
                    if(this->components[t] == compList.size()
                            || compList[this->components[t]]->getEntityId() != nextId) {
                        found = false;
                        break;
                    }
                }
                if(found) {
                    break;
                }
            } while(this->components[0] < this->em->components[this->componentTypes[this->components[0]]].size());
            
            if(this->components[0] >= this->em->components[this->componentTypes[this->components[0]]].size()) {
                this->setToEnd();
            }
            return *this;
        }

        EntityManager::ComponentIterator EntityManager::ComponentIterator::operator++(int) {
            ComponentIterator result(*this);
            this->operator++();
            return result;
        }

        shared_ptr<Component>& EntityManager::ComponentIterator::operator*() {
            return this->operator[](0);
        }

        shared_ptr<Component>& EntityManager::ComponentIterator::operator->() {
            return this->operator[](0);
        }

        shared_ptr<Component>& EntityManager::ComponentIterator::operator[](std::size_t index) {
            return this->em->components[this->componentTypes[index]][this->components[index]];
        }
        
        bool EntityManager::ComponentIterator::operator!=(const ComponentIterator& right) const {
            bool result = !(*this == right); // Reuse equals operator
            return result;
        }
        bool EntityManager::ComponentIterator::operator==(const ComponentIterator& right) const {
            if(this->em != right.em || this->componentTypes.size() != right.componentTypes.size()
                    || this->components.size() != right.components.size()) {
                return false;
            }
            for(size_t i = 0; i < this->components.size(); ++i) {
                if(this->componentTypes[i] != right.componentTypes[i]
                        || this->components[i] != right.components[i]) {
                    return false;
                }
            }
            return true;
        }
    }
}

