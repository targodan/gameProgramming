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
#include "Component.h"

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
        
        class EntityManager : public Serializable {
        protected:
            entityId_t nextEntityId;
            
            Map<componentId_t, vector<shared_ptr<Component>>> components;
            Map<entityId_t, Map<componentId_t, size_t>> entities;
            friend Entity;
                
            Entity createEntity(size_t id, const std::string& name);
            Entity createEntityFromPrefab(const pb::Prefab& msg);
            void addComponent(entityId_t eId, shared_ptr<Component> comp);
            inline size_t getComponentIndexOfEntity(entityId_t eId, componentId_t compId) {
                return this->entities[eId][compId];
            }
            shared_ptr<Component> getComponentOfEntity(entityId_t eId, componentId_t compId);
            bool hasEntityComponent(entityId_t eId, componentId_t compId);
            
            pb::EntityManager msg;
            vector<SerializableComponent*> serializables;
            
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
            Entity createEntityFromPrefab(engine::IO::Serializer& serializer, const std::string& serializedData);
            Entity createEntityFromPrefab(engine::IO::Serializer& serializer, std::istream& serializedData);
            
            void clear() {
                this->components.clear();
                this->entities.clear();
                this->msg.Clear();
                this->nextEntityId = 0;
                this->serializables.clear();
            }
            
            google::protobuf::Message& fromProtobufMessage() override;
            void afterProtobufMessageUpdate() override;
            const google::protobuf::Message& toProtobufMessage() override;
            
            void sort(componentId_t sortBy, std::function<bool(shared_ptr<Component>,shared_ptr<Component>)> comparator) {
                // Sort all except the sortBy component
                for(auto& elem : this->components) {
                    if(elem.first == sortBy) {
                        continue;
                    }
                    std::sort(elem.second.begin(), elem.second.end(), [&,this](const shared_ptr<Component>& l, const shared_ptr<Component>& r) -> bool {
                        if(!this->hasEntityComponent(l->getEntityId(), sortBy)) {
                            return true;
                        }
                        if(!this->hasEntityComponent(r->getEntityId(), sortBy)) {
                            return false;
                        }
                        auto lOrder = this->getComponentOfEntity(l->getEntityId(), sortBy);
                        auto rOrder = this->getComponentOfEntity(r->getEntityId(), sortBy);
                        return bool(comparator(lOrder, rOrder));
                    });
                }
                // Sort the sortBy component
                std::sort(this->components[sortBy].begin(), this->components[sortBy].end(), comparator);
                
                // update all this->entities[*] entries
                for(auto& elem : this->components) {
                    size_t i = 0;
                    for(auto& comp : elem.second) {
                        this->entities[comp->getEntityId()][comp->getComponentId()] = i++;
                    }
                }
            }
            ComponentIterator begin(const std::initializer_list<componentId_t>& componentTypes);
            ComponentIterator end();
        };
    }
}

#endif /* ENTITYMANAGER_H */

