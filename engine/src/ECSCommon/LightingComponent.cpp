#include "LightingComponent.h"

#include "../ECS/ComponentRegisterer.h"

namespace engine {
    namespace ECSCommon {
        ECS_REGISTER_COMPONENT(LightingComponent);
        
        componentId_t LightingComponent::typeId = 0;
        
        componentId_t LightingComponent::getComponentId() const {
            return LightingComponent::typeId;
        }
        std::string LightingComponent::getComponentName() const {
            return "LightingComponent";
        }
        
        std::string LightingComponent::toString() const {
            return "... not gonna try to stuff this into a string.";
        }

        const LightSource& LightingComponent::getLightSource() const {
            return *this->lightSource;
        }
        LightSource& LightingComponent::getLightSource() {
            return *this->lightSource;
        }
        
        void LightingComponent::setComponentTypeId(componentId_t id) {
            LightingComponent::typeId = id;
        }
        componentId_t LightingComponent::getComponentTypeId() {
            return LightingComponent::typeId;
        }
    }
}
