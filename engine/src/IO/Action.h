/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Action.h
 * Author: markus
 *
 * Created on 17. Januar 2017, 20:43
 */

#ifndef ACTION_H
#define ACTION_H

#include "ButtonInfo.h"
#include "../ECS/EntityManager.h"

namespace engine {
    namespace IO {
        
        using engine::ECS::EntityManager;
        
        class Action {
        public:
            Action(int device, int button) {
                bi.buttonID = button;
                bi.deviceID = device;
                bi.xAxis = 0;
                bi.yAxis = 0;
            }
            Action(const Action& orig) {
                bi.buttonID = orig.bi.buttonID;
                bi.deviceID = orig.bi.deviceID;
                bi.xAxis = orig.bi.xAxis;
                bi.yAxis = orig.bi.yAxis;
            }
            
            virtual void execute(EntityManager& em, float deltaT) = 0;
            virtual void setAxes(double x, double y) {
                bi.xAxis = x;
                bi.yAxis = y;
            }
            virtual void setDevButton(int dev, int button) {
                bi.buttonID = button;
                bi.deviceID = dev;
            }
        protected:
            ButtonInfo bi;
        };
    }
}

#endif /* ACTION_H */

