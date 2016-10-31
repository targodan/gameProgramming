#ifndef PLACEMENT_H
#define PLACEMENT_H

#include "../ECS/Component.h"
#include "../math/Vector3.h"

using engine::math::Vector3;
using engine::ECS::compTypeId;

namespace engine {
    namespace ECSCommon {
        class PlacementComponent : public engine::ECS::Component {
        private:
            static compTypeId typeId;

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
            
            compTypeId getComponentTypeId() const override;
            std::string getComponentName() const override;
            std::string toString() const override;

            static void setComponentTypeId(compTypeId id);
        };
    }
}

#endif /* PLACEMENT_H */

