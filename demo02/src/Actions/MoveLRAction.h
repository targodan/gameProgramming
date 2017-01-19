/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MoveLRAction.h
 * Author: markus
 *
 * Created on 19. Januar 2017, 13:11
 */

#ifndef MOVELRACTION_H
#define MOVELRACTION_H


#include "../../../engine/src/ECSCommon/CameraComponent.h"

namespace demo {
    namespace IO {
        
        using engine::IO::Action;
        using engine::ECS::EntityManager;
        using engine::ECSCommon::PlacementComponent;
        using engine::ECSCommon::CameraComponent;
        
        class MoveLRAction : public Action {
        public:
            MoveLRAction(int device, int button, shared_ptr<Entity> player) : Action(device, button), player(player){}
            MoveLRAction(const MoveLRAction& orig) : Action(orig.bi.deviceID, orig.bi.buttonID), player(orig.player){}
            void execute(EntityManager& em) override {
                auto& camera = this->player->getComponent(CameraComponent::getComponentTypeId()).to<CameraComponent>();
                auto& placement = this->player->getComponent(PlacementComponent::getComponentTypeId()).to<PlacementComponent>();
                placement.setVelocityAcc(bi.xAxis * glm::cross(camera.getDirection(), camera.getUp()));
            }
        private:
            shared_ptr<Entity> player;
        };
        
    }
}

#endif /* MOVELRACTION_H */

