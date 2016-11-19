#ifndef PLACEMENT_H
#define PLACEMENT_H

#include <glm/glm.hpp>

#include "../ECS/Component.h"

using engine::ECS::componentId_t;

namespace engine {
    namespace ECSCommon {
        class PlacementComponent : public engine::ECS::Component {
        private:
            static componentId_t typeId;

        protected:
            glm::vec3 position;
            glm::vec3 direction;

        public:
            PlacementComponent();
            PlacementComponent(const PlacementComponent& orig);
            virtual ~PlacementComponent();
            
            const glm::vec3& getPosition() const;
            glm::vec3& getPosition();
            
            void setPosition(const glm::vec3& v);
            
            const glm::vec3& getDirection() const;
            glm::vec3& getDirection();
            
            void setDirection(const glm::vec3& v);
            
            std::string getComponentName() const override;
            std::string toString() const override;

            static void setComponentTypeId(componentId_t id);
            static componentId_t getComponentTypeId();
        };
    }
}

#endif /* PLACEMENT_H */

