#include "CameraComponent.h"
#include "../util/ostream_helper.h"
#include "../ECS/ComponentRegisterer.h"
#include "../math/functions.h"
#include "glm/gtc/matrix_transform.hpp"
#include <sstream>

namespace engine {
    namespace ECSCommon {
        ECS_REGISTER_COMPONENT(CameraComponent);
        
        componentId_t CameraComponent::typeId = 0;
        
        CameraComponent::CameraComponent() {
            
        }
        
        CameraComponent::~CameraComponent() {
            
        }
        
        void CameraComponent::setProjectionMatrix(float horizontalFieldOfView, float aspectRatio, float near, float far) {
            this->projectionMatrix = glm::perspective(horizontalFieldOfView, aspectRatio, near, far);
        }
        
        void CameraComponent::setViewMatrix(vec3 position, vec3 direction, vec3 up) {
            this->viewMatrix = glm::lookAt(position, direction, up);
        }
        
        glm::mat4 CameraComponent::getProjectionMatrix() const {
            return this->projectionMatrix;
        }
        
        glm::mat4 CameraComponent::getViewMatrix() const {
            return this->viewMatrix;
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