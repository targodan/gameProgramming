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
            CameraComponent(); // Default camera in origin, looking in negative z-direction
            CameraComponent(vec3 direction, float horizontalFieldOfView, float aspectRatio, float near, float far);
            CameraComponent(vec3 direction, vec3 up, float horizontalFieldOfView, float aspectRatio, float near, float far);
            CameraComponent(const CameraComponent& orig);
            CameraComponent(CameraComponent&& orig);
            virtual ~CameraComponent();

            void setViewMatrix(const vec3& position, const vec3& direction, const vec3& up);
            void setViewMatrix(const vec3& position);
            void setDirection(const vec3& direction);
            void setUp(const vec3& up);

            CameraComponent& setHorizontalFieldOfView(float fov);
            CameraComponent& setAspectRatio(float ratio);
            CameraComponent& setNearPlane(float distance);
            CameraComponent& setFarPlane(float distance);

            const mat4& getProjectionMatrix() const;
            const mat4& getViewMatrix() const;
            const vec3& getDirection() const;
            const vec3& getUp() const;

            void pan(float xOffset, float yOffset, float sensitivity = 0.1f);

            virtual componentId_t getComponentId() const override;
            virtual std::string getComponentName() const override;
            virtual std::string toString() const override;

            static void setComponentTypeId(componentId_t id);
            static componentId_t getComponentTypeId();
        private:
            void updateProjectionMatrix();

            void updateViewMatrix();
            void setYawAndPitchFromDirection();

            static componentId_t typeId;

            mat4 viewMatrix;  // Look-at transformation
            mat4 projectionMatrix; // Frustum transformation

            vec3 position; // Completely private - only used internally (should always equal player position, which is stored in its PlacementComponent)
            vec3 direction;
            vec3 up;
            const vec3 worldUp; // = {0, 1, 0}

            float yaw; // Rotation around y-axis (~ vertical movement))
            float pitch; // Rotation around x-axis (~ horizontal movement)

            float horizontalFieldOfView;
            float aspectRatio;
            float near;
            float far;
        };
    }
}


#endif /* CAMERACOMPONENT_H */
