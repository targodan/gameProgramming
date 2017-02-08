/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FlyUpDownAction.h
 * Author: markus
 *
 * Created on 24. Januar 2017, 14:55
 */

#ifndef FLYUPDOWNACTION_H
#define FLYUPDOWNACTION_H

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
        
        class FlyUpDownAction : public Action {
        public:
            FlyUpDownAction(int device, int button, shared_ptr<Entity> player) : Action(device, button), player(player){}
            FlyUpDownAction(const FlyUpDownAction& orig) : Action(orig.bi.deviceID, orig.bi.buttonID), player(orig.player){}
            void execute(EntityManager& em) override {
                auto& camera = this->player->getComponent(CameraComponent::getComponentTypeId()).to<CameraComponent>();
                auto& placement = this->player->getComponent(PlacementComponent::getComponentTypeId()).to<PlacementComponent>();
                placement.setVelocityAcc(bi.xAxis * camera.getUp());
            }
        private:
            shared_ptr<Entity> player;
        };
        
    }
}


#endif /* FLYUPDOWNACTION_H */

