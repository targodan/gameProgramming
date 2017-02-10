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
            PlaceBombAction(int device, int button, shared_ptr<Entity> Player, shared_ptr<VisualObject> bomb) : Action(device, button), time(1.f), player(Player), bomb(bomb) {}
            PlaceBombAction(const PlaceBombAction& orig) : Action(orig.bi.deviceID, orig.bi.buttonID), time(orig.time), player(orig.player), bomb(orig.bomb) {}
            void execute(EntityManager& em, float deltaT) override {
                if(time < 1) {
                    time += deltaT;
                }
                else {
                    auto pos = this->player->getComponent<PlacementComponent>().getPosition();
                    auto dir = this->player->getComponent<CameraComponent>().getDirection();
                    LOG(INFO) << (pos + vec3(dir.x, 0, dir.z ));
                    em.createEntityAsync("Bomb", {make_shared<VisualComponent>(bomb), make_shared<PlacementComponent>(pos + vec3(dir.x, 0, dir.z ))} );
                    time = 0;
                }
            }
        private:
            float time;
            shared_ptr<Entity> player;
            shared_ptr<VisualObject> bomb;
        };
    }
}

#endif /* PLACEBOMBACTION_H */

