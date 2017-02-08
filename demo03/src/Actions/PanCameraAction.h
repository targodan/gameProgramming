/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PanCameraAction.h
 * Author: markus
 *
 * Created on 18. Januar 2017, 15:17
 */

#ifndef PANCAMERAACTION_H
#define PANCAMERAACTION_H

#include "../../../engine/src/IO/Action.h"
#include "../../../engine/src/ECS/EntityManager.h"
#include "../../../engine/src/ECSCommon/CameraComponent.h"

namespace demo {
    namespace IO {
        
        using engine::IO::Action;
        using engine::ECS::EntityManager;
        using engine::ECSCommon::CameraComponent;
        
        class PanCameraAction : public Action {
        public:
            PanCameraAction(int device, int button, shared_ptr<Entity> player, float sensibility = 1e-3) : Action(device, button), player(player), sensibility(sensibility){}
            PanCameraAction(const PanCameraAction& orig) : Action(orig.bi.deviceID, orig.bi.buttonID), player(orig.player), sensibility(orig.sensibility){}
            void execute(EntityManager& em) override {
                auto& camera = this->player->getComponent(CameraComponent::getComponentTypeId()).to<CameraComponent>();

                camera.pan(this->bi.xAxis, this->bi.yAxis, sensibility);
            }
        private:
            shared_ptr<Entity> player;
            float sensibility;
        };
        
    }
}

#endif /* PANCAMERAACTION_H */

