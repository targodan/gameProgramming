#include "Camera.h"
#include "math/functions.h"
#include "glm/gtc/matrix_transform.hpp"

namespace engine {
        using engine::math::deg2rad;
        
        Camera::Camera() : position() {

        }
        Camera::Camera(vec3 position) : position(position) {

        }
        Camera::~Camera() {

        }

        void Camera::setViewMatrix(vec3 direction, vec3 up) {
//                vec3 directionNorm = glm::normalize(direction);
//                vec3 upNorm = glm::normalize(up);
//                
//                vec3 rightNorm = glm::normalize(glm::cross(directionNorm, upNorm));
//                upNorm = glm::normalize(glm::cross(rightNorm, directionNorm)); // better safe than sorry
//                
//                glm::mat4 rotation = {
//                    rightNorm.x, upNorm.x, -directionNorm.x, 0, 
//                    rightNorm.y, upNorm.y, -directionNorm.y, 0,
//                    rightNorm.z, upNorm.z, -directionNorm.z, 0,
//                              0,        0,                0, 1
//                };
//                
//                glm::mat4 translation = {
//                                     1,                   0,                   0,  0,
//                                     0,                   1,                   0,  0,
//                                     0,                   0,                   1,  0,
//                    -(this->position.x), -(this->position.y), -(this->position.z), 1
//                };
//                
//                this->viewMatrix = rotation * translation;
            this->direction = direction;
            this->up = up;
            
            this->viewMatrix = glm::lookAt(this->position, direction, up);
        }

        void Camera::setProjectionMatrix(float horizontalFieldOfView, float aspectRatio, float near, float far) {
//                float verticalFieldOfViewRad = deg2rad(verticalFieldOfView);
//                
//                float top = near * glm::tan(verticalFieldOfViewRad / 2.f);
//                float bottom = -top;
//                float right = aspectRatio * top;
//                float left = -right;
//                
//                this->projectionMatrix = glm::frustum(left, right, bottom, top, near, far);
            this->projectionMatrix = glm::perspective(horizontalFieldOfView, aspectRatio, near, far);
        }

        void Camera::setPosition(const vec3 position) {
            this->position = position;
            this->updateViewMatrix();
        }
        
        void Camera::updateViewMatrix() {
            this->setViewMatrix(this->direction, this->up);
        }

        glm::mat4 Camera::getProjectionMatrix() const {
            return this->projectionMatrix;
        }

        glm::mat4 Camera::getViewMatrix() const {
            return this->viewMatrix;
        }

        vec3 Camera::getPosition() const {
            return this->position;
        }
}
