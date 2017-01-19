#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include "util/vec4.h"
#include "util/vec3.h"
#include "glm/mat4x4.hpp"
#include "glm/gtx/string_cast.hpp"
#include "../ECS/Component.h"
#include "../util/vec3.h"

namespace engine {
    namespace ECSCommon {
        using engine::ECS::componentId_t;
        using glm::vec3;
        using glm::mat4;
        
        class CameraComponent : public engine::ECS::Component {
        public:
            CameraComponent();
            CameraComponent(vec3 direction); 
            CameraComponent(vec3 direction, vec3 up);
            CameraComponent(const CameraComponent& orig) 
                : viewMatrix(orig.viewMatrix), projectionMatrix(orig.projectionMatrix), direction(orig.direction), up(orig.up), worldUp(orig.worldUp), yaw(orig.yaw), pitch(orig.pitch) {};
            CameraComponent(CameraComponent&& orig) 
                : viewMatrix(std::move(orig.viewMatrix)), projectionMatrix(std::move(orig.projectionMatrix)), 
                  direction(std::move(orig.direction)), up(std::move(orig.up)), worldUp(std::move(orig.worldUp)), yaw(std::move(orig.yaw)), pitch(std::move(orig.pitch)) {};
            virtual ~CameraComponent();
            
            /* 
             * Field-of-view given in degrees
             */
            void setProjectionMatrix(float horizontalFieldOfView, float aspectRatio, float near, float far);
            void setViewMatrix(const vec3& position, const vec3& direction, const vec3& up);
            void setViewMatrix(const vec3& position);
            void setDirection(const vec3& direction, bool init = false);
            void setUp(const vec3& up);
            
            const mat4& getProjectionMatrix() const;
            const mat4& getViewMatrix() const;
            const vec3& getDirection() const;
            const vec3& getUp() const;
            
            // void setTarget(vec3 targetPosition);
            void pan(float xOffset, float yOffset, float sensitivity = 0.1f);
            
            virtual componentId_t getComponentId() const override;
            virtual std::string getComponentName() const override;
            virtual std::string toString() const override;

            static void setComponentTypeId(componentId_t id);
            static componentId_t getComponentTypeId();
        private:
            void updateViewMatrix();
            
            static componentId_t typeId;
            
            mat4 viewMatrix;  // Look-at transformation
            mat4 projectionMatrix; // Frustum transformation
            
            vec3 position; // Completely private - only used internally (should always equal player position, which is stored in its PlacementComponent)
            vec3 direction;
            vec3 up;
            const vec3 worldUp; // = {0, 1, 0}
            
            float yaw; // Rotation around y-axis (~ vertical movement))
            float pitch; // Rotation around x-axis (~ horizontal movement)
        };
    }
}


#endif /* CAMERACOMPONENT_H */

