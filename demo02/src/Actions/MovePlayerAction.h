/* 
 * File:   MovePlayerAction.h
 * Author: markus
 *
 * Created on 18. Januar 2017, 20:54
 */

#ifndef MOVEPLAYERACTION_H
#define MOVEPLAYERACTION_H

#include "../../../engine/src/IO/Action.h"
#include "../../../engine/src/ECS/EntityManager.h"
#include "../../../engine/src/ECSCommon/PlacementComponent.h"
#include "../../../engine/src/util/vec3.h"

namespace demo {
    namespace IO {
        
        using engine::IO::Action;
        using engine::ECS::EntityManager;
        using engine::ECSCommon::PlacementComponent;
        
        class MovePlayerAction : public Action {
        public:
            MovePlayerAction(int device, int button, shared_ptr<Entity> player) : Action(device, button), player(player){}
            MovePlayerAction(const MovePlayerAction& orig) : Action(orig.bi.deviceID, orig.bi.buttonID), player(orig.player){}
            void execute(EntityManager& em) override {
                auto& placement = this->player->getComponent(PlacementComponent::getComponentTypeId()).to<PlacementComponent>();
                if(this->bi.buttonID == GLFW_KEY_W) {
                    placement.setDirection(vec3(0.1,0,0));
                }
                else if(this->bi.buttonID == GLFW_KEY_S) {
                    placement.setDirection(vec3(-0.1,0,0));
                }
                else if(this->bi.buttonID == GLFW_KEY_A) {
                    placement.setDirection(vec3(0,-0.1,0));
                }
                else if(this->bi.buttonID == GLFW_KEY_D) {
                    placement.setDirection(vec3(0,0.1,0));
                }
                else {
                    placement.setDirection(vec3(0,0,0))
                }
            }
        private:
            shared_ptr<Entity> player;
        };
        
    }
}

#endif /* MOVEPLAYERACTION_H */

