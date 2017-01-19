#include "TimerComponent.h"

#include "../ECS/ComponentRegisterer.h"

#include <sstream>

namespace engine {
    namespace ECSCommon {
        ECS_REGISTER_COMPONENT(TimerComponent);
        
        componentId_t TimerComponent::typeId = 0;
        
        componentId_t TimerComponent::getComponentId() const {
            return TimerComponent::typeId;
        }
        
        std::string TimerComponent::getComponentName() const {
            return "TimerComponent";
        }
        
        std::string TimerComponent::toString() const {
            std::stringstream ss;
            ss << this->time;
            return ss.str();
        }

        void TimerComponent::setComponentTypeId(componentId_t id) {
            TimerComponent::typeId = id;
        }
        
        componentId_t TimerComponent::getComponentTypeId() {
            return TimerComponent::typeId;
        }
    }
}
