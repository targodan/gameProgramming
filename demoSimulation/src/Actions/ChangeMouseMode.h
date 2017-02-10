/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ChangeMouseMode.h
 * Author: markus
 *
 * Created on 24. Januar 2017, 13:50
 */

#ifndef CHANGEMOUSEMODE_H
#define CHANGEMOUSEMODE_H

#include "../../../engine/src/IO/Action.h"
#include "../../../engine/src/ECS/EntityManager.h"

namespace demo {
    namespace IO {
        
        using engine::IO::Action;
        using engine::ECS::EntityManager;
        
        class ChangeMouseMode : public Action {
        public:
            ChangeMouseMode(int device, int button, GLFWwindow* window) : Action(device, button), window(window) {}
            ChangeMouseMode(const ChangeMouseMode& orig) : Action(orig.bi.deviceID, orig.bi.buttonID), window(orig.window) {}
            void execute(EntityManager& em, float deltaT) override {
                if(this->isDisabledMode) {
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                } else {
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                }
                this->isDisabledMode = !this->isDisabledMode;
            }
        private:
            GLFWwindow* window;
            bool isDisabledMode = false;
        };
    }
}

#endif /* CHANGEMOUSEMODE_H */

