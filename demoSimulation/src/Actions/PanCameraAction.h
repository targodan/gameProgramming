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

#include <memory>

#include "../../../engine/src/IO/Action.h"
#include "../../../engine/src/ECS/EntityManager.h"
#include "../../../engine/src/ECSCommon/CameraComponent.h"

namespace demo {
    namespace IO {
        
        using engine::IO::Action;
        using engine::ECS::Entity;
        using engine::ECS::EntityManager;
        using engine::ECSCommon::CameraComponent;
        using std::shared_ptr;
        
        class PanCameraAction : public Action {
        public:
            PanCameraAction(int device, int button, shared_ptr<Entity> player) : Action(device, button), player(player){}
            PanCameraAction(const PanCameraAction& orig) : Action(orig.bi.deviceID, orig.bi.buttonID), player(orig.player){}
            void execute(EntityManager& em) override {
                auto& camera = this->player->getComponent(CameraComponent::getComponentTypeId()).to<CameraComponent>();
                camera.pan(this->bi.xAxis, this->bi.yAxis, 1e-3);
            }
        private:
            shared_ptr<Entity> player;
        };
        
    }
}

#endif /* PANCAMERAACTION_H */

