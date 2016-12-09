#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <memory>
#include <cstring>
#include <initializer_list>
#include <functional>
#include <string>
#include <iostream>

#include "../WTFException.h"
#include "../util/Array.h"
#include "../util/Map.h"
#include "../util/vector.h"
#include "../IO/Serializable.h"
#include "../IO/Serializer.h"
#include "pb/EntityManager.pb.h"
#include "SerializableComponent.h"

#include "EntityId.h"

namespace engine {
    namespace ECS {
        using engine::util::vector;
        using std::shared_ptr;
        using std::weak_ptr;
        using std::unique_ptr;
        using engine::util::Map;
        using engine::util::Array;
        using engine::IO::Serializable;

        class Entity;
        class Component;
        
        class EntityManager : public Serializable {
        protected:
            entityId_t nextEntityId;
            
            Map<componentId_t, vector<shared_ptr<Component>>> m_components;
            Map<entityId_t, Map<componentId_t, size_t>> entityComponentIndexes;
            Map<std::string, Entity> m_entities;
            friend Entity;
                
            Entity createEntity(size_t id, const std::string& name);
            Entity createEntityFromPrefab(const pb::Prefab& msg);
            void addComponent(entityId_t eId, shared_ptr<Component> component);
            inline size_t getComponentIndexOfEntity(entityId_t eId, componentId_t compId) {
                return this->entityComponentIndexes[eId][compId];
            }
            shared_ptr<Component> getComponentOfEntity(entityId_t eId, componentId_t compId);
            bool doesEntityContainComponentOfType(entityId_t eId, componentId_t compId);
            
            pb::EntityManager protobufMessage;
            vector<SerializableComponent*> serializables;
            
        public:
            class ComponentIterator : public std::iterator<std::input_iterator_tag, shared_ptr<Component>> {
            private:
                EntityManager* em;
                Array<componentId_t> componentTypes;
                Array<size_t> componentIndexes;
                bool isEnd;
               
                ComponentIterator(EntityManager* em,
                        const Array<componentId_t>& componentTypes);
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
            Entity createEntityFromPrefab(engine::IO::Serializer& serializer, const std::string& serializedData);
            Entity createEntityFromPrefab(engine::IO::Serializer& serializer, std::istream& serializedData);
            void getAllEntites() const;
            
            void clear();
            
            google::protobuf::Message& fromProtobufMessage() override;
            void afterProtobufMessageUpdate() override;
            const google::protobuf::Message& toProtobufMessage() override;
            
            void sort(componentId_t sortBy, std::function<bool(shared_ptr<Component>,shared_ptr<Component>)> comparator);
            
            class ComponentsProxy {
            protected:
                EntityManager* em;
                Array<componentId_t> componentTypes;
                
            public:
                ComponentsProxy(EntityManager* em, const std::initializer_list<componentId_t>& componentTypes)
                : em(em), componentTypes(componentTypes) {}
                
                ComponentIterator begin();
                ComponentIterator end();
            };
            
            ComponentsProxy components(const std::initializer_list<componentId_t>& componentTypes);
        };
    }
}

#endif /* ENTITYMANAGER_H */

