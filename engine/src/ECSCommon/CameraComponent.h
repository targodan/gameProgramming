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
            CameraComponent(const CameraComponent& orig) = delete;
            CameraComponent(CameraComponent&& orig) = delete;
            virtual ~CameraComponent();
            
            /* 
             * Field-of-view given in degrees
             */
            void setProjectionMatrix(float horizontalFieldOfView, float aspectRatio, float near, float far);
            void setViewMatrix(vec3 position, vec3 direction, vec3 up);
            
            glm::mat4 getProjectionMatrix() const;
            glm::mat4 getViewMatrix() const;
            
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

