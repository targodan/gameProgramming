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
        using util::vec3;
        using glm::mat4;
        
        class CameraComponent : public engine::ECS::Component {
        public:
            CameraComponent();
            CameraComponent(vec3 direction, vec3 up);
            CameraComponent(const CameraComponent& orig) 
                : viewMatrix(orig.viewMatrix), projectionMatrix(orig.projectionMatrix), direction(orig.direction), up(orig.up) {};
            CameraComponent(CameraComponent&& orig) 
                : viewMatrix(std::move(orig.viewMatrix)), projectionMatrix(std::move(orig.projectionMatrix)), direction(std::move(orig.direction)), up(std::move(orig.up)) {};
            virtual ~CameraComponent();
            
            /* 
             * Field-of-view given in degrees
             */
            void setProjectionMatrix(float horizontalFieldOfView, float aspectRatio, float near, float far);
            void setViewMatrix(const vec3& position, const vec3& direction, const vec3& up);
            void setViewMatrix(const vec3& position);
            void setDirection(const vec3& direction);
            void setUp(const vec3& up);
            
            const mat4& getProjectionMatrix() const;
            const mat4& getViewMatrix() const;
            const vec3& getDirection() const;
            const vec3& getUp() const;
            
            void setTarget(vec3 targetPosition);
            void pan(float verticalDegrees, float horizontalDegrees);
            
            virtual componentId_t getComponentId() const override;
            virtual std::string getComponentName() const override;
            virtual std::string toString() const override;

            static void setComponentTypeId(componentId_t id);
            static componentId_t getComponentTypeId();
        private:
            static componentId_t typeId;
            
            mat4 viewMatrix;  // Look-at transformation
            mat4 projectionMatrix; // Frustum transformation
            
            vec3 direction;
            vec3 up;
        };
    }
}


#endif /* CAMERACOMPONENT_H */

