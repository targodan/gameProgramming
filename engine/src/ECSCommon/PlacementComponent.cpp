#include "PlacementComponent.h"

#include <sstream>

#include "../util/ostream_helper.h"

#include "../ECS/ComponentRegisterer.h"

namespace engine {
    namespace ECSCommon {
        ECS_REGISTER_COMPONENT(PlacementComponent);
        
        componentId_t PlacementComponent::typeId = 0;
        
        PlacementComponent::PlacementComponent() {
        }

        PlacementComponent::PlacementComponent(const PlacementComponent& orig) {
        }

        PlacementComponent::~PlacementComponent() {
        }

        const glm::vec3& PlacementComponent::getPosition() const {
            return this->position;
        }
        
        glm::vec3& PlacementComponent::getPosition() {
            return this->position;
        }
        
        void PlacementComponent::setPosition(const glm::vec3& v) {
            this->position = v;
        }

        const glm::vec3& PlacementComponent::getDirection() const {
            return this->direction;
        }
        
        glm::vec3& PlacementComponent::getDirection() {
            return this->direction;
        }
            
        void PlacementComponent::setDirection(const glm::vec3& v) {
            this->direction = v;
        }
            
        std::string PlacementComponent::getComponentName() const {
            return "Placement";
        }
        
        std::string PlacementComponent::toString() const {
            std::stringstream ss;
            ss << "{" << "Position: " << this->position
                    << ", Direction: " << this->direction
                    << "}";
            return ss.str();
        }

        void PlacementComponent::setComponentTypeId(componentId_t id) {
            PlacementComponent::typeId = id;
        }

        componentId_t PlacementComponent::getComponentTypeId() {
            return PlacementComponent::typeId;
        }
    }
}