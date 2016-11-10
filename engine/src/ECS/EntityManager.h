#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <vector>
#include <memory>
#include <cstring>
#include <map>
#include <initializer_list>

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
            // TODO: don't use std::map!!!!
            // TODO: do it with unique_ptr instead of shared. Tip: use std::move.
            map<componentId_t, vector<shared_ptr<Component>>> components;
            
        public:
            class ComponentIterator {
            private:
                EntityManager& em;
                std::size_t numTypes;
                int* componentTypes = nullptr;
                shared_ptr<Component>* components = nullptr;
               
                ComponentIterator(EntityManager& em,
                        const std::initializer_list<componentId_t>& componentTypes);
                
                friend class EntityManager;
                
            public:
                ComponentIterator(const ComponentIterator& ci);
                ComponentIterator(ComponentIterator&& ci);
                ~ComponentIterator();
                
                ComponentIterator& operator++();
                ComponentIterator operator++(int);
                
                shared_ptr<Component> operator*();
                shared_ptr<Component> operator->();
                shared_ptr<Component> operator[](std::size_t index);
            };

            EntityManager();
            EntityManager(const EntityManager& orig);
            virtual ~EntityManager();
            
            Entity addEntity(const std::string& name);
            
            ComponentIterator getComponentIterator(const std::initializer_list<componentId_t>& componentTypes);
        };
    }
}

#endif /* ENTITYMANAGER_H */

