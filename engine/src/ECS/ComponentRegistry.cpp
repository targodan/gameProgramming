#include "ComponentRegistry.h"
#include "UnknownComponentException.h"
#include "ComponentRegisterer.h"
#include "CollisionException.h"

engine::ECS::componentId_t engine::ECS::ComponentRegistry::nextId = 0;
std::unique_ptr<engine::util::Map<engine::ECS::componentId_t, engine::ECS::ComponentInstantiator*>> engine::ECS::ComponentRegistry::instantiators = nullptr;
std::unique_ptr<engine::util::Map<engine::ECS::componentId_t, engine::ECS::ProtobufMessageInstantiator*>> engine::ECS::ComponentRegistry::messageInstantiators = nullptr;

namespace engine {
    namespace ECS {
        void ComponentRegistry::registerInstantiator(componentId_t id, ComponentInstantiator* ci) {
            if(ComponentRegistry::instantiators == nullptr) {
                ComponentRegistry::instantiators = std::make_unique<Map<componentId_t, ComponentInstantiator*>>();
                ComponentRegistry::instantiators->set_empty_key(SIZE_MAX);
            }
            (*ComponentRegistry::instantiators)[id] = ci;
        }
        
        void ComponentRegistry::registerProtobufMessageInstantiator(componentId_t id, ProtobufMessageInstantiator* ci) {
            if(ComponentRegistry::messageInstantiators == nullptr) {
                ComponentRegistry::messageInstantiators = std::make_unique<Map<componentId_t, ProtobufMessageInstantiator*>>();
                ComponentRegistry::messageInstantiators->set_empty_key(SIZE_MAX);
            }
            (*ComponentRegistry::messageInstantiators)[id] = ci;
            ComponentRegistry::registerInstantiator(id, dynamic_cast<ComponentInstantiator*>(ci));
        }
        
        Component* ComponentRegistry::makeComponentOfType(componentId_t id) {
            auto it = ComponentRegistry::instantiators->find(id);
            if(it == ComponentRegistry::instantiators->end()) {
                throw UnknownComponentException("Component of type %zu is not registered!", id);
            }
            return it->second->instantiate();
        }
        
        google::protobuf::Message* ComponentRegistry::makeProtobufMessageForComponentOfType(componentId_t id) {
            auto it = ComponentRegistry::messageInstantiators->find(id);
            if(it == ComponentRegistry::messageInstantiators->end()) {
                throw UnknownComponentException("Component of type %zu is not registered!", id);
            }
            return it->second->instantiateProtobufMessage();
        }
    }
}