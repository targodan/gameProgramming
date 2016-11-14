#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <vector>
#include <memory>
#include <cstring>
#include <initializer_list>

#include "../WTFException.h"
#include "../util/Array.h"
#include "../util/Map.h"

#include "EntityId.h"
#include "Component.h"

using std::vector;
using std::shared_ptr;
using std::weak_ptr;
using std::unique_ptr;
using engine::util::Map;
using engine::util::Array;

namespace engine {
    namespace ECS {
        class Entity;
        
        class EntityManager {
        protected:
            entityId_t nextEntityId;
            
            // INVARIANT: The vectors are always sorted by entity id.
            Map<componentId_t, vector<shared_ptr<Component>>> components;
            friend Entity;
                
            void addComponent(const Entity& e, const shared_ptr<Component>& comp);
            
        public:
            class ComponentIterator : public std::iterator<std::input_iterator_tag, shared_ptr<Component>> {
            private:
                EntityManager* em;
                Array<componentId_t> componentTypes;
                Array<size_t> components;
                bool isEnd;
               
                ComponentIterator(EntityManager* em,
                        const std::initializer_list<componentId_t>& componentTypes);
                ComponentIterator(EntityManager* em);
                
                void setToEnd();
                friend class EntityManager;
            public:
                ComponentIterator(const ComponentIterator& ci);
                ComponentIterator(ComponentIterator&& ci);
                ~ComponentIterator();
                
                ComponentIterator& operator++();
                ComponentIterator operator++(int);
                
                shared_ptr<Component>& operator*();
                shared_ptr<Component>& operator->();
                shared_ptr<Component>& operator[](std::size_t index);
                
                bool operator!=(const ComponentIterator& right) const;
                bool operator==(const ComponentIterator& right) const;
                
                void swap(ComponentIterator& ci);
            };

            EntityManager();
            EntityManager(const EntityManager& orig) = delete;
            virtual ~EntityManager();
            
            Entity createEntity(const std::string& name);
            
            ComponentIterator begin(const std::initializer_list<componentId_t>& componentTypes);
            ComponentIterator end();
        };
    }
}

#endif /* ENTITYMANAGER_H */

