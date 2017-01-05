#ifndef CAMERA_H
#define CAMERA_H

#include "util/vec4.h"
#include "util/vec3.h"
#include "glm/mat4x4.hpp"
#include "glm/gtx/string_cast.hpp"

namespace engine {
    namespace renderer {
        using glm::vec3;
        
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
            void setProjectionMatrix(float verticalFieldOfView, float aspectRatio, float near, float far);
            void setModelViewMatrix(vec3 direction, vec3 up);
            void setPosition(const vec3 position);
            
            glm::mat4 getProjectionMatrix() const;
            glm::mat4 getModelViewMatrix() const;
            vec3 getPosition() const;
            
            friend std::ostream& operator<<(std::ostream& os, const Camera& camera) {
                os << "Camera at " << glm::to_string(camera.position) << " with " << std::endl
                        << "ModelViewMatrix: " << glm::to_string(camera.modelViewMatrix)
                        << std::endl << "and ProjectionMatrix: " << glm::to_string(camera.projectionMatrix) 
                        << std::endl;
                return os;
            }
        private:
            vec3 position;
            
            glm::mat4 modelViewMatrix;  // Look-at transformation
            glm::mat4 projectionMatrix; // Frustum transformation
        };       
    }
}


#endif /* CAMERA_H */

