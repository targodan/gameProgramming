#include "EntityManager.h"

#include "Entity.h"
#include "ComponentRegistry.h"

namespace engine {
    namespace ECS {
        EntityManager::EntityManager() : nextEntityId(0) {
            this->components.set_empty_key(SIZE_MAX);
            this->entityComponentIndexes.set_empty_key(SIZE_MAX);
        }

        EntityManager::~EntityManager() {
        }
        
        Entity EntityManager::createEntity(const std::string& name) {
            return this->createEntity(this->nextEntityId++, name);
        }
        
        Entity EntityManager::createEntity(size_t id, const std::string& name) {
            auto ret = Entity(id, this, name);
            this->entityComponentIndexes[ret.getId()].set_empty_key(SIZE_MAX);
            return ret;
        }
        
        Entity EntityManager::createEntityFromPrefab(const pb::Prefab& prefab) {
            Entity ret = this->createEntity(prefab.entity_name());
            
            for(auto& compMsg : prefab.components()) {
                // Create Component
                auto component = std::shared_ptr<Component>(ComponentRegistry::constructComponentOfType(compMsg.component_type_name()));
                
#ifdef DEBUG
                try {
#endif
                    // Must be SerializableComponent
                    auto& serialiable = component->to<SerializableComponent>();
                
                    // Update the Component
                    auto& msg = serialiable.fromProtobufMessage();
                    compMsg.component().UnpackTo(&msg);
                    serialiable.afterProtobufMessageUpdate();
#ifdef DEBUG
                } catch(...) {
                    throw WTFException("Components in Prefabs must be SerializableComponents. Component of type \"%s\" not a SerializableComponent.", compMsg.component_type_name().c_str());
                }
#endif
                
                this->addComponent(ret.getId(), component);
            }
            
            return ret;
        }
        
        Entity EntityManager::createEntityFromPrefab(engine::IO::Serializer& serializer, const std::string& serializedData) {
            pb::Prefab prefab;
            serializer.deserialize(prefab, serializedData);
            return this->createEntityFromPrefab(prefab);
        }
        
        Entity EntityManager::createEntityFromPrefab(engine::IO::Serializer& serializer, std::istream& serializedData) {
            pb::Prefab prefab;
            serializer.deserialize(prefab, serializedData);
            return this->createEntityFromPrefab(prefab);
        }
        
        void EntityManager::addComponent(entityId_t eId, shared_ptr<Component> component) {
            auto& vec = this->components[component->getComponentId()];
            vec.push_back(component);
            
            auto itMap = this->entityComponentIndexes.find(eId);
            auto& map = this->entityComponentIndexes[eId];
            if(itMap == this->entityComponentIndexes.end()) {
                map.set_empty_key(SIZE_MAX);
            }
            map[component->getComponentId()] = vec.size()-1;
            
            SerializableComponent* serializable = dynamic_cast<SerializableComponent*>(component.get());
            if(serializable != nullptr) /* is of type SerializableComponent */ {
                this->serializables.push_back(serializable);
            }
        }
        
        google::protobuf::Message& EntityManager::fromProtobufMessage() {
            return this->protobufMessage;
        }
        
        const google::protobuf::Message& EntityManager::toProtobufMessage() {
            this->protobufMessage.set_next_entity_id(this->nextEntityId);
            for(auto& elem : this->serializables) {
                google::protobuf::Any* wrapper = new google::protobuf::Any();
                wrapper->PackFrom(elem->toProtobufMessage());

                auto compMsg = this->protobufMessage.add_components();
                compMsg->set_entity_id(elem->getEntityId());
                compMsg->set_component_type_name(ComponentRegistry::getComponentTypeName(elem->getComponentId()));
                compMsg->set_allocated_component(wrapper);
            }
            return this->protobufMessage;
        }
        
        void EntityManager::afterProtobufMessageUpdate() {
            // Prevent future collisions
            if(this->nextEntityId < this->protobufMessage.next_entity_id()) {
                this->nextEntityId = this->protobufMessage.next_entity_id();
            }
            
            // Iterate over all components
            for(const auto& compMsg : this->protobufMessage.components()) {
                auto itE = this->entityComponentIndexes.find(compMsg.entity_id());
                // Does the entity exists yet?
                if(itE == this->entityComponentIndexes.end()) {
                    // No => Create entity
                    this->createEntity(compMsg.entity_id(), "FromSerialization");
                    itE = this->entityComponentIndexes.find(compMsg.entity_id());
                }
                // Does the entity already have a component of this type?
                auto componentTypeId = ComponentRegistry::getComponentTypeId(compMsg.component_type_name());
                if(itE->second.find(componentTypeId) == itE->second.end()) {
                    // No => Create it.
                    this->addComponent(compMsg.entity_id(), std::shared_ptr<Component>(ComponentRegistry::constructComponentOfType(componentTypeId)));
                }
                // Retrieve the Component (has to be a SerializableComponent)
                auto& comp = this->getComponentOfEntity(compMsg.entity_id(), componentTypeId)
                                    ->to<SerializableComponent>();
                // Update the Component
                auto& msg = comp.fromProtobufMessage();
                compMsg.component().UnpackTo(&msg);
                comp.afterProtobufMessageUpdate();
            }
            this->protobufMessage.Clear();
        }
        
        shared_ptr<Component> EntityManager::getComponentOfEntity(entityId_t eId, componentId_t compId) {
            return this->components[compId][this->getComponentIndexOfEntity(eId, compId)];
        }
        
        bool EntityManager::doesEntityContainComponentOfType(entityId_t eId, componentId_t compId) {
            return this->entityComponentIndexes[eId].find(compId) != this->entityComponentIndexes[eId].end();
        }
            
        EntityManager::ComponentIterator EntityManager::begin(const std::initializer_list<componentId_t>& componentTypes) {
            for(auto id : componentTypes) {
                auto it = this->components.find(id);
                if(it == this->components.end() || it->second.size() == 0) {
                    return this->end();
                }
            }
            return ComponentIterator(this, componentTypes);
        }
        
        EntityManager::ComponentIterator EntityManager::end() {
            auto it = ComponentIterator(this);
            return it;
        }
        
        
        EntityManager::ComponentIterator::ComponentIterator(EntityManager* em,
                const std::initializer_list<componentId_t>& componentTypes)
                : em(em), componentTypes(componentTypes), componentIndexes(componentTypes.size()), isEnd(false) {
        }
        
        EntityManager::ComponentIterator::ComponentIterator(EntityManager* em)
                : em(em), componentTypes(0), componentIndexes(0), isEnd(true) {
        }

        EntityManager::ComponentIterator::ComponentIterator(const ComponentIterator& ci)
                : em(ci.em), componentTypes(ci.componentTypes), componentIndexes(ci.componentIndexes), isEnd(ci.isEnd) {
        }

        EntityManager::ComponentIterator::ComponentIterator(ComponentIterator&& ci)
                : em(ci.em), componentTypes(std::move(ci.componentTypes))
                    , componentIndexes(std::move(ci.componentIndexes)), isEnd(std::move(ci.isEnd)) {}

        EntityManager::ComponentIterator::~ComponentIterator() {}
        
        void EntityManager::ComponentIterator::swap(ComponentIterator& ci) {
            std::swap(this->em, ci.em);
            std::swap(this->componentIndexes, ci.componentIndexes);
            std::swap(this->componentTypes, ci.componentTypes);
            std::swap(this->isEnd, ci.isEnd);
        }
        
        void EntityManager::ComponentIterator::setToEnd() {
            this->isEnd = true;
        }

        EntityManager::ComponentIterator& EntityManager::ComponentIterator::operator++() {
            if(this->isEnd) {
                // Already at end.
                return *this;
            }
            while(true) {
                ++this->componentIndexes[0];
                if(this->componentIndexes[0] >= this->em->components[this->componentTypes[0]].size()) {
                    this->setToEnd();
                    break;
                }
                auto eId = this->operator[](0)->getEntityId();
                bool allGood = true;
                for(size_t i = 1; i < this->componentTypes.size(); ++i) {
                    if(!this->em->doesEntityContainComponentOfType(eId, this->componentTypes[i])) {
                        allGood = false;
                        break;
                    }
                    this->componentIndexes[i] = this->em->getComponentIndexOfEntity(eId, this->componentTypes[i]);
                    if(this->componentIndexes[i] >= this->em->components[this->componentTypes[i]].size()) {
                        allGood = false;
                        break;
                    }
                }
                if(allGood) {
                    break;
                }
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
            return this->em->components[this->componentTypes[index]][this->componentIndexes[index]];
        }
        
        bool EntityManager::ComponentIterator::operator!=(const ComponentIterator& right) const {
            if(this->isEnd != right.isEnd) {
                return true;
            }
            bool result = !(*this == right); // Reuse equals operator
            return result;
        }
        bool EntityManager::ComponentIterator::operator==(const ComponentIterator& right) const {
            if(this->em != right.em) {
                return false;
            }
            if(this->isEnd == right.isEnd) {
                return true;
            }
            if(this->componentTypes.size() != right.componentTypes.size()
                    || this->componentIndexes.size() != right.componentIndexes.size()) {
                return false;
            }
            for(size_t i = 0; i < this->componentIndexes.size(); ++i) {
                if(this->componentTypes[i] != right.componentTypes[i]
                        || this->componentIndexes[i] != right.componentIndexes[i]) {
                    return false;
                }
            }
            return true;
        }
    }
}

