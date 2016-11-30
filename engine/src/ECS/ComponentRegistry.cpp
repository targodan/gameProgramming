#include "ComponentRegistry.h"
#include "UnknownComponentException.h"
#include "ComponentRegisterer.h"
#include "CollisionException.h"

engine::ECS::componentId_t engine::ECS::ComponentRegistry::nextId = 0;
std::unique_ptr<engine::util::Map<engine::ECS::componentId_t, engine::ECS::ComponentInstantiator*>> engine::ECS::ComponentRegistry::instantiators = nullptr;

namespace engine {
    namespace ECS {
        void ComponentRegistry::registerInstantiator(componentId_t id, ComponentInstantiator* ci) {
            if(ComponentRegistry::instantiators == nullptr) {
                ComponentRegistry::instantiators = std::make_unique<Map<componentId_t, ComponentInstantiator*>>();
                ComponentRegistry::instantiators->set_empty_key(SIZE_MAX);
            }
            (*ComponentRegistry::instantiators)[id] = ci;
        }
        
        Component* ComponentRegistry::makeComponentOfType(componentId_t id) {
            auto it = ComponentRegistry::instantiators->find(id);
            if(it == ComponentRegistry::instantiators->end()) {
                throw UnknownComponentException("Component of type %zu is not registered!", id);
            }
            return it->second->instantiate();
        }
    }
}