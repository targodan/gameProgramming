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
        class ActionMessage : Message {
        protected:
            int deviceID, buttonID;
            double xAxis, yAxis;
            
        public:
            ActionMessage(messageId_t id, int device, int button, double xAxis = 0, double yAxis = 0) : 
            Message(id),
            deviceID(device),
            buttonID(button),
            xAxis(xAxis),
            yAxis(yAxis)
            {}
            
            ActionMessage(const Message& message, int device, int button, double xAxis, double yAxis) :
            Message(message),
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
            
        };
    }
}

#endif /* ACTIONMESSAGE_H */

