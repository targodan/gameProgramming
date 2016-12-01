#include "ComponentRegistry.h"
#include "UnknownComponentException.h"
#include "ComponentRegisterer.h"
#include "CollisionException.h"
#include "../WTFException.h"

engine::ECS::componentId_t engine::ECS::ComponentRegistry::nextId = 0;
std::unique_ptr<engine::util::Map<engine::ECS::componentId_t, engine::ECS::ComponentRegistry::CompInfo>> engine::ECS::ComponentRegistry::registeredComponents = nullptr;
std::unique_ptr<engine::util::Map<std::string, engine::ECS::componentId_t>> engine::ECS::ComponentRegistry::componentNames = nullptr;

namespace engine {
    namespace ECS {
        void ComponentRegistry::registerComponent(componentId_t id, std::string name, ComponentInstantiator* ci) {
            if(ComponentRegistry::registeredComponents == nullptr) {
                ComponentRegistry::registeredComponents = std::make_unique<Map<componentId_t, ComponentRegistry::CompInfo>>();
                ComponentRegistry::registeredComponents->set_empty_key(SIZE_MAX);
            }
            if(ComponentRegistry::componentNames == nullptr) {
                ComponentRegistry::componentNames = std::make_unique<Map<std::string, componentId_t>>();
                ComponentRegistry::componentNames->set_empty_key("");
            }
            if(ComponentRegistry::registeredComponents->find(id) != ComponentRegistry::registeredComponents->end()) {
                throw WTFException("Component id %zu is ambiguous! WTF? How did you manage this?!", id);
            }
            if(ComponentRegistry::componentNames->find(name) != ComponentRegistry::componentNames->end()) {
                throw CollisionException("Component name %s is ambiguous!", name.c_str());
            }
            (*ComponentRegistry::registeredComponents)[id] = ComponentRegistry::CompInfo(name, ci);
            (*ComponentRegistry::componentNames)[name] = id;
        }
        
        Component* ComponentRegistry::makeComponentOfType(componentId_t id) {
            auto it = ComponentRegistry::registeredComponents->find(id);
            if(it == ComponentRegistry::registeredComponents->end()) {
                throw UnknownComponentException("Component of type %zu is not registered!", id);
            }
            return it->second.instantiator->instantiate();
        }
        
        Component* ComponentRegistry::makeComponentOfType(std::string name) {
            auto it = ComponentRegistry::componentNames->find(name);
            if(it == ComponentRegistry::componentNames->end()) {
                throw UnknownComponentException("Component of name %s is not registered!", name.c_str());
            }
            return ComponentRegistry::makeComponentOfType(it->second);
        }
        
        componentId_t ComponentRegistry::getComponentTypeId(std::string name) {
            auto it = ComponentRegistry::componentNames->find(name);
            if(it == ComponentRegistry::componentNames->end()) {
                throw UnknownComponentException("Component of name %s is not registered!", name.c_str());
            }
            return it->second;
        }
        
        std::string ComponentRegistry::getComponentTypeName(componentId_t id) {
            auto it = ComponentRegistry::registeredComponents->find(id);
            if(it == ComponentRegistry::registeredComponents->end()) {
                throw UnknownComponentException("Component of type %zu is not registered!", id);
            }
            return it->second.name;
        }
    }
}