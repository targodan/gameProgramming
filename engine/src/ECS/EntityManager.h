#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <vector>
#include <memory>
#include <cstring>
#include <map>

#include "../WTFException.h"

#include "Component.h"
#include "Entity.h"

using std::vector;
using std::map;
using std::shared_ptr;

namespace engine {
    namespace ECS {
        class EntityManager {
        private:
            map<std::size_t, shared_ptr<Component>> components;
            
            class ComponentIterator {
            private:
                EntityManager& em;
                std::size_t numTypes;
                int* componentTypes = nullptr;
                shared_ptr<Component>* components = nullptr;
               
            public:
                ComponentIterator(EntityManager& em,
                        std::initializer_list<int> componentTypes) : em(em) {
                    this->numTypes = componentTypes.size();
                    this->componentTypes = new int[this->numTypes];
                    this->components = new shared_ptr<Component>[this->numTypes];
                    std::copy(componentTypes.begin(), componentTypes.end(), this->componentTypes);
                }
                
                ComponentIterator(const ComponentIterator& ci) : em(ci.em) {
                    this->componentTypes = new int[ci.numTypes];
                    this->components = new shared_ptr<Component>[this->numTypes];
                    std::copy(ci.componentTypes, ci.componentTypes + sizeof(int) * ci.numTypes, this->componentTypes);
                }
                
                ComponentIterator(ComponentIterator&& ci) : em(ci.em) {
                    this->componentTypes = ci.componentTypes;
                    this->components = ci.components;
                }
                
                ~ComponentIterator() {
                    if(this->componentTypes != nullptr) {
                        delete[] this->componentTypes;
                    }
                    if(this->components != nullptr) {
                        delete[] this->components;
                    }
                }
                
                ComponentIterator& operator++() {
                    // Do actual increment
                    return *this;
                }

                ComponentIterator operator++(int) {
                    ComponentIterator result(*this);
                    this->operator++();
                    return result;
                }
                
                shared_ptr<Component> operator*() {
                    return this->operator[](0);
                }

                shared_ptr<Component> operator->() {
                    return this->operator[](0);
                }
                
                shared_ptr<Component> operator[](std::size_t index) {
#ifdef DEBUG
                    if(index >= this->numTypes) {
                        throw WTFException("You did not request enough types.");
                    }
#endif
                    return this->components[index];
                }
            };

        public:
            EntityManager();
            EntityManager(const EntityManager& orig);
            virtual ~EntityManager();
            
            Entity addEntity(const std::string& name);
            
            ComponentIterator getComponentIterator(std::initializer_list<int> componentTypes);
        };
    }
}

#endif /* ENTITYMANAGER_H */

