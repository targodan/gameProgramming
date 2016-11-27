#ifndef PLACEMENT_H
#define PLACEMENT_H

#include <glm/glm.hpp>

#include "../ECS/Component.h"
#include "../util/vec3.h"

#include "../IO/Serializable.h"
#include "pb/Placement.pb.h"

namespace engine {
    namespace ECSCommon {
        using engine::ECS::componentId_t;
        using engine::util::vec3;
        
        class PlacementComponent : public engine::ECS::Component, public engine::IO::Serializable<pb::Placement> {
        private:
            static componentId_t typeId;

        protected:
            vec3 position;
            vec3 direction;
            
            pb::Placement msg;
            
            google::protobuf::Message& getProtobufMessage() override;

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

