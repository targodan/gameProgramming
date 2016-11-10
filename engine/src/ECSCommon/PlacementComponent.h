#ifndef PLACEMENT_H
#define PLACEMENT_H

#include "../ECS/Component.h"
#include "../math/Vector3.h"

using engine::math::Vector3;
using engine::ECS::componentId_t;

namespace engine {
    namespace ECSCommon {
        class PlacementComponent : public engine::ECS::Component {
        private:
            static componentId_t typeId;

        protected:
            Vector3 position;
            Vector3 direction;

        public:
            PlacementComponent();
            PlacementComponent(const PlacementComponent& orig);
            virtual ~PlacementComponent();
            
            const Vector3& getPosition() const;
            Vector3& getPosition();
            
            void setPosition(const Vector3& v);
            
            const Vector3& getDirection() const;
            Vector3& getDirection();
            
            void setDirection(const Vector3& v);
            
            std::string getComponentName() const override;
            std::string toString() const override;

            static void setComponentTypeId(componentId_t id);
            static componentId_t getComponentTypeId();
        };
    }
}

#endif /* PLACEMENT_H */

