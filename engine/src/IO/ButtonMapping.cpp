/* 
 * File:   ButtonMapping.cpp
 * Author: markus
 * 
 * Created on 27. Dezember 2016, 14:13
 */

#include "ButtonMapping.h"
namespace engine {
    namespace IO {
        ButtonMapping::ButtonMapping(GLFWwindow* window, shared_ptr<MessageHandler> handler, int dev) : 
        window(window),
        handler(handler),
        device(dev) {
            this->mapping.set_empty_key(SIZE_MAX);
        }

        ButtonMapping::~ButtonMapping() {
        }
        
        void ButtonMapping::insertMapping(int buttonId, shared_ptr<Message> msg){
            if(this->mapping.find(buttonId) == this->mapping.end()) {
                this->mapping[buttonId] = msg;
            }
        }
        
        void ButtonMapping::deleteMapping(int buttonId) {
            this->mapping.erase(buttonId);
        }
        
        void ButtonMapping::deleteMapping(shared_ptr<Message> msg) {
            for(auto it = this->mapping.begin(); it != this->mapping.end(); it++) {
                if(it->second->getId() == msg->getId()) {
                    this->mapping.erase(it);
                }
            }
        }
        
         void ButtonMapping::queueMessages() {
            if(this->device == -1) {
                for(auto it = this->mapping.begin(); it != this->mapping.end(); it++) {
                    if(glfwGetKey(this->window, it->first) == GLFW_PRESS) {
                        this->handler->queueMessage(it->second);
                    }
                }
            }
            else {
                int count;
                const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);
                
                for(auto it = this->mapping.begin(); it != this->mapping.end(); it++) {
                    if(it->first < count && buttons[it->first] == GLFW_PRESS) {
                        this->handler->queueMessage(it->second);
                    }
                }
            }
        }
    }
}
