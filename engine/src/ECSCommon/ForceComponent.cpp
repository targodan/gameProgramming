#include "ForceComponent.h"

#include "../ECS/ComponentRegisterer.h"

namespace engine {
    namespace ECSCommon {
        ECS_REGISTER_COMPONENT(ForceComponent);
        
        componentId_t ForceComponent::typeId = 0;
        
        componentId_t ForceComponent::getComponentId() const {
            return ForceComponent::typeId;
        }
        
        std::string ForceComponent::getComponentName() const {
            return "ForceComponent";
        }
        
        std::string ForceComponent::toString() const {
            return "... not gonna try to stuff this into a string.";
        }

        void ForceComponent::setComponentTypeId(componentId_t id) {
            ForceComponent::typeId = id;
        }
        
        componentId_t ForceComponent::getComponentTypeId() {
            return ForceComponent::typeId;
        }
    }
}
