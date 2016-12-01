#include "EntityManager.h"

#include "Entity.h"
#include "ComponentRegistry.h"

namespace engine {
    namespace ECS {
        EntityManager::EntityManager() : nextEntityId(0) {
            this->components.set_empty_key(SIZE_MAX);
            this->entities.set_empty_key(SIZE_MAX);
        }
            entityId_t nextEntityId;
            
            Map<componentId_t, vector<shared_ptr<Component>>> components;
            Map<entityId_t, Map<componentId_t, size_t>> entities;
            pb::EntityManager msg;
            vector<SerializableComponent*> serializables;

        EntityManager::~EntityManager() {
        }
        
        Entity EntityManager::createEntity(const std::string& name) {
            return this->createEntity(this->nextEntityId++, name);
        }
        
        Entity EntityManager::createEntity(size_t id, const std::string& name) {
            auto ret = Entity(id, this, name);
            this->entities[ret.getId()].set_empty_key(SIZE_MAX);
            return ret;
        }
        
        void EntityManager::addComponent(entityId_t eId, shared_ptr<Component> comp) {
            auto& vec = this->components[comp->getComponentId()];
            vec.push_back(comp);
            
            auto itMap = this->entities.find(eId);
            auto& map = this->entities[eId];
            if(itMap == this->entities.end()) {
                map.set_empty_key(SIZE_MAX);
            }
            map[comp->getComponentId()] = vec.size()-1;
            
            SerializableComponent* sc = dynamic_cast<SerializableComponent*>(comp.get());
            if(sc != nullptr) /* is of type SerializableComponent */ {
                this->serializables.push_back(sc);
            }
        }
        
        google::protobuf::Message& EntityManager::fromProtobufMessage() {
            return this->msg;
        }
        
        const google::protobuf::Message& EntityManager::toProtobufMessage() {
            this->msg.set_next_entity_id(this->nextEntityId);
            for(auto& elem : this->serializables) {
                google::protobuf::Any* wrapper = new google::protobuf::Any();
                wrapper->PackFrom(elem->toProtobufMessage());

                auto compMsg = this->msg.add_components();
                compMsg->set_entity_id(elem->getEntityId());
                compMsg->set_component_type_name(ComponentRegistry::getComponentTypeName(elem->getComponentId()));
                compMsg->set_allocated_child(wrapper);
            }
            return this->msg;
        }
        
        void EntityManager::afterProtobufMessageUpdate() {
            // Prevent future collisions
            if(this->nextEntityId < this->msg.next_entity_id()) {
                this->nextEntityId = this->msg.next_entity_id();
            }
            
            // Iterate over all components
            for(const auto& compMsg : this->msg.components()) {
                auto itE = this->entities.find(compMsg.entity_id());
                // Does the entity exists yet?
                if(itE == this->entities.end()) {
                    // No => Create entity
                    this->createEntity(compMsg.entity_id(), "FromSerialization");
                    itE = this->entities.find(compMsg.entity_id());
                }
                // Does the entity already have a component of this type?
                auto componentTypeId = ComponentRegistry::getComponentTypeId(compMsg.component_type_name());
                if(itE->second.find(componentTypeId) == itE->second.end()) {
                    // No => Create it.
                    this->addComponent(compMsg.entity_id(), std::shared_ptr<Component>(ComponentRegistry::makeComponentOfType(componentTypeId)));
                }
                // Retrieve the Component (has to be a SerializableComponent)
                auto& comp = this->getComponentOfEntity(compMsg.entity_id(), componentTypeId)
                                    ->to<SerializableComponent>();
                // Update the Component
                auto& msg = comp.fromProtobufMessage();
                compMsg.child().UnpackTo(&msg);
                comp.afterProtobufMessageUpdate();
            }
            this->msg.Clear();
        }
        
        shared_ptr<Component> EntityManager::getComponentOfEntity(entityId_t eId, componentId_t compId) {
            return this->components[compId][this->getComponentIndexOfEntity(eId, compId)];
        }
        
        bool EntityManager::hasEntityComponent(entityId_t eId, componentId_t compId) {
            return this->entities[eId].find(compId) != this->entities[eId].end();
        }
            
        EntityManager::ComponentIterator EntityManager::begin(const std::initializer_list<componentId_t>& componentTypes) {
            return ComponentIterator(this, componentTypes);
        }
        
        EntityManager::ComponentIterator EntityManager::end() {
            auto it = ComponentIterator(this);
            return it;
        }
        
        
        EntityManager::ComponentIterator::ComponentIterator(EntityManager* em,
                const std::initializer_list<componentId_t>& componentTypes)
                : em(em), componentTypes(componentTypes), components(componentTypes.size()), isEnd(false) {
        }
        
        EntityManager::ComponentIterator::ComponentIterator(EntityManager* em)
                : em(em), componentTypes(0), components(0), isEnd(true) {
        }

        EntityManager::ComponentIterator::ComponentIterator(const ComponentIterator& ci)
                : em(ci.em), componentTypes(ci.componentTypes), components(ci.components), isEnd(ci.isEnd) {
        }

        EntityManager::ComponentIterator::ComponentIterator(ComponentIterator&& ci)
                : em(ci.em), componentTypes(std::move(ci.componentTypes))
                    , components(std::move(ci.components)), isEnd(std::move(ci.isEnd)) {}

        EntityManager::ComponentIterator::~ComponentIterator() {}
        
        void EntityManager::ComponentIterator::swap(ComponentIterator& ci) {
            std::swap(this->em, ci.em);
            std::swap(this->components, ci.components);
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
                ++this->components[0];
                if(this->components[0] >= this->em->components[this->componentTypes[0]].size()) {
                    this->setToEnd();
                    break;
                }
                auto eId = this->operator[](0)->getEntityId();
                bool allGood = true;
                for(size_t i = 1; i < this->componentTypes.size(); ++i) {
                    if(!this->em->hasEntityComponent(eId, this->componentTypes[i])) {
                        allGood = false;
                        break;
                    }
                    this->components[i] = this->em->getComponentIndexOfEntity(eId, this->componentTypes[i]);
                    if(this->components[i] >= this->em->components[this->componentTypes[i]].size()) {
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
            return this->em->components[this->componentTypes[index]][this->components[index]];
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

