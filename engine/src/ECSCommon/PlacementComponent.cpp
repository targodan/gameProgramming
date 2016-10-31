#include "PlacementComponent.h"

#include "../ECS/ComponentRegisterer.h"

namespace engine {
    namespace ECSCommon {
        ECS_REGISTER_COMPONENT(PlacementComponent);
        
        compTypeId PlacementComponent::typeId = 0;
        
        PlacementComponent::PlacementComponent() {
        }

        PlacementComponent::PlacementComponent(const PlacementComponent& orig) {
        }

        PlacementComponent::~PlacementComponent() {
        }

        const Vector3& PlacementComponent::getPosition() const {
            return this->position;
        }
        
        Vector3& PlacementComponent::getPosition() {
            return this->position;
        }
        
        void PlacementComponent::setPosition(const Vector3& v) {
            this->position = v;
        }

        const Vector3& PlacementComponent::getDirection() const {
            return this->direction;
        }
        
        Vector3& PlacementComponent::getDirection() {
            return this->direction;
        }
            
        void PlacementComponent::setDirection(const Vector3& v) {
            this->direction = v;
        }
            
        compTypeId PlacementComponent::getComponentTypeId() const {
            return PlacementComponent::typeId;
        }
        
        std::string PlacementComponent::getComponentName() const {
            return "Placement";
        }
        
        std::string PlacementComponent::toString() const {
            std::stringstream ss;
            ss << "{" << "Position: " << this->position.toString()
                    << ", Direction: " << this->direction.toString()
                    << "}";
            return ss.str();
        }

        void PlacementComponent::setComponentTypeId(compTypeId id) {
            PlacementComponent::typeId = id;
        }
    }
}