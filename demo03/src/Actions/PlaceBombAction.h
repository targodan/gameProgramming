/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PlaceBombAction.h
 * Author: markus
 *
 * Created on 2. Februar 2017, 16:25
 */

#ifndef PLACEBOMBACTION_H
#define PLACEBOMBACTION_H

#include "../../../engine/src/IO/Action.h"
#include "../../../engine/src/ECS/EntityManager.h"
#include "../../../engine/src/ECS/Entity.h"
#include "../../../engine/src/ECSCommon/CameraComponent.h"
#include "../../../engine/src/ECSCommon/PlacementComponent.h"
#include "../../../engine/src/ECSCommon/VisualComponent.h"

namespace demo {
    namespace IO {
        
        using engine::IO::Action;
        using engine::ECS::EntityManager;
        using engine::ECS::Entity;
        using engine::ECSCommon::PlacementComponent;
        using engine::ECSCommon::CameraComponent;
        using engine::ECSCommon::VisualComponent;
        
        class PlaceBombAction : public Action {
        public:
            PlaceBombAction(int device, int button, shared_ptr<Entity> Player) : Action(device, button), prev(false), player(Player) {}
            PlaceBombAction(const PlaceBombAction& orig) : Action(orig.bi.deviceID, orig.bi.buttonID), player(orig.player) {}
            void execute(EntityManager& em) override {
                if(!prev) {
                    auto pos = this->player->getComponent<PlacementComponent>().getPosition();
                    auto dir = this->player->getComponent<CameraComponent>().getDirection();
                    
                    
                    VisualObject bombMesh = {"resources/models/bomb.blend"};
                    bombMesh.loadObject();
                    
                    em.createEntityAsync("Bomb", {make_shared<VisualComponent>(std::make_shared<VisualObject>(bombMesh)), make_shared<PlacementComponent>(pos + vec3(dir.x, 0, dir.z ))} );
                }
            }
        private:
            bool prev;
            shared_ptr<Entity> player;
        };
    }
}

#endif /* PLACEBOMBACTION_H */

