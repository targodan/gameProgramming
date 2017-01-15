/* 
 * File:   ButtonMapping.cpp
 * Author: markus
 * 
 * Created on 27. Dezember 2016, 14:13
 */

#include "ButtonMapping.h"
namespace engine {
    namespace IO {
        ButtonMapping::ButtonMapping(GLFWwindow* window, MessageHandler& handler) : 
        handler(handler),
        window(window)
        {
            devButton empty;
            empty.buttonID = -1000;
            empty.deviceID = -1000;
            this->mapping.set_empty_key(empty);
        }

        ButtonMapping::~ButtonMapping() {
        }
        
        void ButtonMapping::insertMapping(int deviceID, int buttonID, shared_ptr<Message> msg){
            devButton newSrc;
            newSrc.deviceID = deviceID;
            newSrc.buttonID = buttonID;
            if(this->mapping.find(newSrc) == this->mapping.end()) {
                this->mapping[newSrc] = msg;
            }
        }
        
        void ButtonMapping::deleteMapping(int deviceID, int buttonID) {
            devButton oldSrc;
            oldSrc.deviceID = deviceID;
            oldSrc.buttonID = buttonID;
            this->mapping.erase(oldSrc);
        }
        
        void ButtonMapping::deleteMapping(shared_ptr<Message> msg) {
            for(auto it = this->mapping.begin(); it != this->mapping.end(); it++) {
                if(it->second->getId() == msg->getId()) {
                    this->mapping.erase(it);
                }
            }
        }
        
         void ButtonMapping::queueMessages() {
             
            int prevDevice = -1000;
            int bCount, aCount;
            const unsigned char* buttons;
            const float* axis;
            
            for(auto it = this->mapping.begin(); it != this->mapping.end(); it++) {
                
                if(it->first.deviceID == KEYBOARD) {
                    if(glfwGetKey(this->window, it->first.buttonID) == GLFW_PRESS) {
                        this->handler.queueMessage(it->second);
                    }
                }
                
                else if(it->first.deviceID == MOUSE) {
                    if(it->first.buttonID == -1) {
                        double xpos, ypos;
                        glfwGetCursorPos(this->window, &xpos, &ypos);
                        it->second->setAxes(xpos, ypos);
                        this->handler.queueMessage(it->second);
                    }
                    if(glfwGetMouseButton(this->window, it->first.buttonID) == GLFW_PRESS) {
                        this->handler.queueMessage(it->second);
                    }
                }
                
                else if(it->first.deviceID >= 0)
                {
                    if(prevDevice != it->first.deviceID) {
                        buttons = glfwGetJoystickButtons(it->first.deviceID, &bCount);
                        axis = glfwGetJoystickAxes(it->first.deviceID, &aCount);
                    }
                    if(it->first.buttonID < 0 && (it->first.buttonID*2) > -aCount) {
                        double xpos = axis[-(it->first.buttonID)*2-1];
                        double ypos = axis[-(it->first.buttonID)*2];
                        it->second->setAxes(xpos, ypos);
                        this->handler.queueMessage(it->second);
                    }
                    if(it->first.buttonID < bCount && buttons[it->first.buttonID] == GLFW_PRESS) {
                        this->handler.queueMessage(it->second);
                    }
                }
                prevDevice = it->first.deviceID;
            }
        }
    }
}
