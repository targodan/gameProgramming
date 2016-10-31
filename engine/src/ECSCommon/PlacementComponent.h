#ifndef PLACEMENT_H
#define PLACEMENT_H

#include "../ECS/Component.h"
#include "../math/Vector3.h"

using engine::math::Vector3;

namespace engine {
    namespace ECSCommon {
        class PlacementComponent : public engine::ECS::Component {
        private:
            static int typeId;

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
            
            int getComponentTypeId() const;
            std::string getComponentName() const;
            std::string toString() const;

            static void setComponentTypeId(int id);
        };
    }
}

#endif /* PLACEMENT_H */

