#include "CameraComponent.h"
#include "../util/ostream_helper.h"
#include "../ECS/ComponentRegisterer.h"
#include "../math/functions.h"
#include "glm/gtc/matrix_transform.hpp"
#include "../WTFException.h"
#include "CameraException.h"
#include <sstream>
#include "glm/gtx/euler_angles.hpp"
#include <iostream>
#include "glm/gtx/string_cast.hpp"

namespace engine {
    namespace ECSCommon {
        using engine::math::deg2rad;
        
        ECS_REGISTER_COMPONENT(CameraComponent);
        
        componentId_t CameraComponent::typeId = 0;
        
        CameraComponent::CameraComponent() 
            : position(vec3{0.f, 0.f, 0.f}), direction(vec3{-1.f, 0.f, 0.f}), worldUp(vec3{0.f, 1.f, 0.f}), yaw(-180.f), pitch(0.f) {
            this->up = this->worldUp;
        }
        CameraComponent::CameraComponent(vec3 direction) 
            : position(vec3{0.f, 0.f, 0.f}), direction(direction), worldUp(vec3{0.f, 1.f, 0.f}), yaw(-180.f), pitch(0.f) {
            auto right = glm::normalize(glm::cross(this->direction, this->worldUp));
            this->up = glm::normalize(glm::cross(right, this->direction));
        }
        CameraComponent::CameraComponent(vec3 direction, vec3 up) 
            : position(vec3{0.f, 0.f, 0.f}), worldUp(vec3{0.f, 1.f, 0.f}), yaw(-180.f), pitch(0.f) {
            if(glm::dot(direction, up) != 0) {
                throw CameraException("Given direction and up vectors are not orthogonal.");
            }
            
           
            this->setDirection(direction);
            this->setUp(up);
        }
        
        CameraComponent::~CameraComponent() {
            
        }
        
        void CameraComponent::setProjectionMatrix(float horizontalFieldOfView, float aspectRatio, float near, float far) {
            this->projectionMatrix = glm::perspective(horizontalFieldOfView, aspectRatio, near, far);
        }
        void CameraComponent::setViewMatrix(const vec3& position, const vec3& direction, const vec3& up) {
            if(glm::dot(direction, up) != 0) {
                throw CameraException("Given direction and up vectors are not orthogonal.");
            }
            
            this->setDirection(direction);
            this->setUp(up);
            this->setViewMatrix(position);
        }
        void CameraComponent::setViewMatrix(const vec3& position) {
            this->position = position;
            this->updateViewMatrix();
        }
        void CameraComponent::updateViewMatrix() {
            this->viewMatrix = glm::lookAt(this->position, this->position+this->direction, this->up);
        }
        void CameraComponent::setDirection(const vec3& direction) {
            this->direction = direction;
        }
        void CameraComponent::setUp(const vec3& up) {
            this->up = glm::normalize(up);
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
            
            this->pitch = this->pitch > 90.f ? 90.f : this->pitch;
            this->pitch = this->pitch < -90.f ? -90.f : this->pitch;
            
            this->direction.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
            this->direction.y = sin(glm::radians(this->pitch));
            this->direction.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
            
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
