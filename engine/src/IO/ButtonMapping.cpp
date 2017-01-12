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
             int count;
             const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);
            
            for(auto it = this->mapping.begin(); it != this->mapping.end(); it++) {
                
                if(it->first.deviceID == KEYBOARD) {
                    if(glfwGetKey(this->window, it->first) == GLFW_PRESS) {
                        this->handler.queueMessage(it->second);
                    }
                }
                
                else if(it->first.deviceID == MOUSE){
                    
                }
                
                else if(it->first.deviceID >= 0)
                {
                    if(it->first.buttonID < count && buttons[it->first.buttonID] == GLFW_PRESS) {
                    this->handler.queueMessage(it->second);
                    }
                }
            }
        }
    }
}
