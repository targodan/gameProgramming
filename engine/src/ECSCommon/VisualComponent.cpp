#include "VisualComponent.h"
#include "../ECS/ComponentRegisterer.h"

#include <sstream>

namespace engine {
    namespace ECSCommon {
        ECS_REGISTER_COMPONENT(VisualComponent);
        
        componentId_t VisualComponent::typeId = 0;
        
        VisualComponent::VisualComponent() {}
        VisualComponent::VisualComponent(const Mesh& mesh, const Material& material) 
            : object(VisualObject{mesh, material}) {}
        VisualComponent::VisualComponent(const VisualObject& object) : object(object) {}
        VisualComponent::~VisualComponent() {}
        
        void VisualComponent::setVisualObject(const VisualObject& object) {
            this->object = object;
        }
        const VisualObject& VisualComponent::getVisualObject() const {
            return this->object;
        }
        VisualObject& VisualComponent::getVisualObject() {
            return this->object;
        }
        
        componentId_t VisualComponent::getComponentId() const {
            return VisualComponent::typeId;
        }
        std::string VisualComponent::getComponentName() const {
            return "Visual";
        }
        std::string VisualComponent::toString() const {
            std::stringstream ss;
            ss << "{" << "VisualComponent" << "}";
            return ss.str();
        }
        
        void VisualComponent::setComponentTypeId(componentId_t id) {
            VisualComponent::typeId = id;
        }
        
        componentId_t VisualComponent::getComponentTypeId() {
            return VisualComponent::typeId;
        }
        
    }
}
