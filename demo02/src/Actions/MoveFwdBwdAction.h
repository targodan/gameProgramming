/* 
 * File:   MovePlayerAction.h
 * Author: markus
 *
 * Created on 18. Januar 2017, 20:54
 */

#ifndef MOVEFWDBWDACTION_H
#define MOVEFWDBWDACTION_H

#include "../../../engine/src/IO/Action.h"
#include "../../../engine/src/ECS/EntityManager.h"
#include "../../../engine/src/ECSCommon/PlacementComponent.h"
#include "../../../engine/src/ECSCommon/CameraComponent.h"
#include "../../../engine/src/util/vec3.h"

namespace demo {
    namespace IO {
        
        using engine::IO::Action;
        using engine::ECS::EntityManager;
        using engine::ECSCommon::PlacementComponent;
        using engine::ECSCommon::CameraComponent;
        
        class MoveFwdBwdAction : public Action {
        public:
            MoveFwdBwdAction(int device, int button, shared_ptr<Entity> player) : Action(device, button), player(player){}
            MoveFwdBwdAction(const MoveFwdBwdAction& orig) : Action(orig.bi.deviceID, orig.bi.buttonID), player(orig.player){}
            void execute(EntityManager& em) override {
                auto& camera = this->player->getComponent(CameraComponent::getComponentTypeId()).to<CameraComponent>();
                auto& placement = this->player->getComponent(PlacementComponent::getComponentTypeId()).to<PlacementComponent>();
                auto dir = camera.getDirection();
                dir.y = 0;
                placement.setVelocityAcc(bi.xAxis * dir);
            }
        private:
            shared_ptr<Entity> player;
        };
        
    }
}

#endif /* MOVEFWDBWDACTION_H */

