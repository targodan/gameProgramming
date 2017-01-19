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
        PlacementComponent::PlacementComponent(const PlacementComponent& orig) : position(orig.position), velocity(orig.velocity), lastVelocity(orig.lastVelocity) {
        }
        PlacementComponent::PlacementComponent(PlacementComponent&& orig) : position(std::move(orig.position)), velocity(std::move(orig.velocity)), lastVelocity(std::move(orig.lastVelocity)) {
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

        const glm::vec3& PlacementComponent::getVelocity() const {
            return this->velocity;
        }
        
        glm::vec3& PlacementComponent::getVelocity() {
            return this->velocity;
        }
            
        void PlacementComponent::setVelocity(const glm::vec3& v) {
            this->velocity = v;
        }
        
        void PlacementComponent::setVelocityAcc(const glm::vec3 v) { 
            this->velocity += v;
        }
        
        const glm::vec3& PlacementComponent::getLastVelocity() const {
            return this->lastVelocity;
        }
        
        glm::vec3& PlacementComponent::getLastVelocity() {
            return this->velocity;
        }
        
        void PlacementComponent::setLastVelocity(const glm::vec3& v) {
            this->lastVelocity = v;
        }
        
        google::protobuf::Message& PlacementComponent::fromProtobufMessage() {
            return this->msg;
        }
        
        const google::protobuf::Message& PlacementComponent::toProtobufMessage() {
            this->msg.set_allocated_position(this->position.toProtobufMessage());
            this->msg.set_allocated_direction(this->velocity.toProtobufMessage());
            return this->msg;
        }
        
        void PlacementComponent::afterProtobufMessageUpdate() {
            this->position.fromProtobufMessage(this->msg.position());
            this->velocity.fromProtobufMessage(this->msg.direction());
        }
            
        std::string PlacementComponent::getComponentName() const {
            return "Placement";
        }
        
        std::string PlacementComponent::toString() const {
            std::stringstream ss;
            ss << "{" << "Position: " << this->position
                    << ", Direction: " << this->velocity
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