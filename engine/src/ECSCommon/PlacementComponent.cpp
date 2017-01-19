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
        PlacementComponent::PlacementComponent(const PlacementComponent& orig) : position(orig.position), direction(orig.direction) {
        }
        PlacementComponent::PlacementComponent(PlacementComponent&& orig) : position(std::move(orig.position)), direction(std::move(orig.direction)) {
        }
        PlacementComponent::~PlacementComponent() {
        }

        const glm::vec3& PlacementComponent::getPosition() const {
            return this->position;
        }
        
        glm::vec3& PlacementComponent::getPosition() {
            return this->position;
        }
        
        void PlacementComponent::move(const glm::vec3& v) {
            this->position += v;
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
        
        google::protobuf::Message& PlacementComponent::fromProtobufMessage() {
            return this->msg;
        }
        
        const google::protobuf::Message& PlacementComponent::toProtobufMessage() {
            this->msg.set_allocated_position(this->position.toProtobufMessage());
            this->msg.set_allocated_direction(this->direction.toProtobufMessage());
            return this->msg;
        }
        
        void PlacementComponent::afterProtobufMessageUpdate() {
            this->position.fromProtobufMessage(this->msg.position());
            this->direction.fromProtobufMessage(this->msg.direction());
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
        
        componentId_t PlacementComponent::getComponentId() const {
            return PlacementComponent::typeId;
        }

        void PlacementComponent::setComponentTypeId(componentId_t id) {
            PlacementComponent::typeId = id;
        }

        componentId_t PlacementComponent::getComponentTypeId() {
            return PlacementComponent::typeId;
        }
    }
}