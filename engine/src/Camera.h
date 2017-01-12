#ifndef CAMERA_H
#define CAMERA_H

#include "util/vec4.h"
#include "util/vec3.h"
#include "glm/mat4x4.hpp"
#include "glm/gtx/string_cast.hpp"

namespace engine {
    using util::vec3;

    class Camera {
    public:
        Camera();
        Camera(vec3 position);
        Camera(const Camera& orig) = delete;
        Camera(Camera&& orig) = delete;
        virtual ~Camera();
        
        /* 
         * Field-of-view given in degrees
         */
        void setProjectionMatrix(float horizontalFieldOfView, float aspectRatio, float near, float far);
        void setViewMatrix(vec3 direction, vec3 up);
        
        void setPosition(const vec3 position);
        void setTarget(const vec3 target);
        void moveBy(const vec3 moveVector);

        glm::mat4 getProjectionMatrix() const;
        glm::mat4 getViewMatrix() const;
        vec3 getPosition() const;

        friend std::ostream& operator<<(std::ostream& os, const Camera& camera) {
            os << "Camera at " << camera.position << " with " << std::endl
                    << "ModelViewMatrix: " << glm::to_string(camera.viewMatrix)
                    << std::endl << "and ProjectionMatrix: " << glm::to_string(camera.projectionMatrix) 
                    << std::endl;
            return os;
        }
    private:
        vec3 position;
        
        vec3 direction;
        vec3 up;

        glm::mat4 viewMatrix;  // Look-at transformation
        glm::mat4 projectionMatrix; // Frustum transformation
        
        void updateViewMatrix();
    };       

}


#endif /* CAMERA_H */

