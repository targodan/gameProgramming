#include "DeformableBodyComponent.h"

#include "../ECS/ComponentRegisterer.h"

namespace engine {
    namespace ECSCommon {
        ECS_REGISTER_COMPONENT(DeformableBodyComponent);
        
        componentId_t DeformableBodyComponent::typeId = 0;
        
        componentId_t DeformableBodyComponent::getComponentId() const {
            return DeformableBodyComponent::typeId;
        }
        
        std::string DeformableBodyComponent::getComponentName() const {
            return "DeformableBodyComponent";
        }
        
        std::string DeformableBodyComponent::toString() const {
            return "... not gonna try to stuff this into a string.";
        }

        void DeformableBodyComponent::setComponentTypeId(componentId_t id) {
            DeformableBodyComponent::typeId = id;
        }
        
        componentId_t DeformableBodyComponent::getComponentTypeId() {
            return DeformableBodyComponent::typeId;
        }
    }
}
