/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ActionMessage.h
 * Author: markus
 *
 * Created on 13. Januar 2017, 18:25
 */

#ifndef ACTIONMESSAGE_H
#define ACTIONMESSAGE_H

namespace engine {
    namespace ECS {
        class ActionMessage : public Message {
        protected:
            int actionID, deviceID, buttonID;
            double xAxis, yAxis;
            
        public:
            ActionMessage(messageId_t id, int action, int device, int button, double xAxis = 0, double yAxis = 0) : 
            Message(id),
            actionID(action),
            deviceID(device),
            buttonID(button),
            xAxis(xAxis),
            yAxis(yAxis)
            {}
            
            ActionMessage(const Message& message, int action, int device, int button, double xAxis = 0, double yAxis = 0) :
            Message(message),
            actionID(action),
            deviceID(device),
            buttonID(button),
            xAxis(xAxis),
            yAxis(yAxis)
            {}
            
            int getDevice() { return deviceID; }
            int getButton() { return deviceID; }
            int getXAxis() { return xAxis; }
            int getYAxis() { return yAxis; }
            bool isAnalog() { return !(xAxis == 0 && yAxis == 0); }
            void setAxes(double xAxis, double yAxis) {
                this->xAxis = xAxis;
                this->yAxis = yAxis;
            }
            
        };
    }
}

#endif /* ACTIONMESSAGE_H */

