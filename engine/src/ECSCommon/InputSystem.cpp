/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   InputSystem.cpp
 * Author: markus
 * 
 * Created on 18. Januar 2017, 12:47
 */

#include "InputSystem.h"

namespace engine {
    namespace ECSCommon {
        
        ECS_REGISTER_SYSTEM(InputSystem);
        
        systemId_t InputSystem::systemId = 0;
        
        InputSystem::InputSystem(const ButtonMapping& bm) : map(bm) {
        }

        InputSystem::~InputSystem() {
        }
        
        void InputSystem::run(EntityManager& em, float deltaTimeSeconds) {
            auto actions = map.getActions();
            for(auto actionptr : actions) {
                actionptr->execute(em, deltaTimeSeconds);
            }
        }
        
        Array<systemId_t> InputSystem::getOptionalDependencies() const {
            return {/* GravitySystem (?) */};
        }
        
        systemId_t InputSystem::getSystemTypeId() const {
            return InputSystem::systemId;
        }
        
        systemId_t InputSystem::systemTypeId() {
            return InputSystem::systemId;
        }
        
        void InputSystem::setSystemTypeId(systemId_t id) {
            InputSystem::systemId = id;
        }

    }
}

