#include "CameraComponent.h"
#include "../util/ostream_helper.h"
#include "../ECS/ComponentRegisterer.h"
#include "../math/functions.h"
#include "glm/gtc/matrix_transform.hpp"
#include "../WTFException.h"
#include <sstream>

namespace engine {
    namespace ECSCommon {
        using engine::math::deg2rad;
        
        ECS_REGISTER_COMPONENT(CameraComponent);
        
        componentId_t CameraComponent::typeId = 0;
        
        CameraComponent::CameraComponent() {
            
        }
        CameraComponent::CameraComponent(vec3 direction) 
            : direction(direction), worldUp(vec3{0.f, 1.f, 0.f}) {
                this->up = this->worldUp;
        }
        CameraComponent::CameraComponent(vec3 direction, vec3 up) 
            : direction(direction), up(up), worldUp(vec3{0.f, 1.f, 0.f}) {
            
        }
        
        CameraComponent::~CameraComponent() {
            
        }
        
        void CameraComponent::setProjectionMatrix(float horizontalFieldOfView, float aspectRatio, float near, float far) {
            this->projectionMatrix = glm::perspective(horizontalFieldOfView, aspectRatio, near, far);
        }
        void CameraComponent::setViewMatrix(const vec3& position, const vec3& direction, const vec3& up) {
            this->direction = direction;
            this->up = up;
            this->setViewMatrix(position);
        }
        void CameraComponent::setViewMatrix(const vec3& position) {
            this->position = position;
            this->viewMatrix = glm::lookAt(position, this->direction, this->up);
        }
        void CameraComponent::updateViewMatrix() {
            this->viewMatrix = glm::lookAt(this->position, this->direction, this->up);
        }
        void CameraComponent::setDirection(const vec3& direction) {
            this->direction = direction;
        }
        void CameraComponent::setUp(const vec3& up) {
            this->up = up;
        }
        
        const mat4& CameraComponent::getProjectionMatrix() const {
            return this->projectionMatrix;
        }
        const mat4& CameraComponent::getViewMatrix() const {
            return this->viewMatrix;
        }
        const vec3& CameraComponent::getDirection() const {
            return this->direction;
        }
        const vec3& CameraComponent::getUp() const {
            return this->up;
        }
        
        void CameraComponent::pan(float xOffset, float yOffset, float sensitivity) {
            // Idea: learnopengl.com
            
            xOffset *= sensitivity;
            yOffset *= sensitivity;
            
            this->yaw += xOffset;
            this->pitch += yOffset;
            
            this->yaw = this->yaw > 90.f ? 90.f : this->yaw;
            this->pitch = this->pitch > 90.f ? 90.f : this->pitch;
            
            this->direction.x = cos(deg2rad(this->yaw)) * cos(deg2rad(this->pitch));
            this->direction.y = sin(deg2rad(this->pitch));
            this->direction.z = sin(deg2rad(this->yaw)) * cos(deg2rad(this->pitch));
            this->direction = glm::normalize(this->direction);
            
            auto right = glm::normalize(glm::cross(this->direction, this->worldUp));
            this->up = glm::normalize(glm::cross(right, this->direction));
            
            this->updateViewMatrix();
        }
        
        std::string CameraComponent::getComponentName() const {
            return "Camera";
        }
        
        std::string CameraComponent::toString() const {
            std::stringstream ss;
            ss << "{" << "CameraComponent" << "}";
            return ss.str();
        }
        
        componentId_t CameraComponent::getComponentId() const {
            return CameraComponent::typeId;
        }

        void CameraComponent::setComponentTypeId(componentId_t id) {
            CameraComponent::typeId = id;
        }

        componentId_t CameraComponent::getComponentTypeId() {
            return CameraComponent::typeId;
        }
    }
}