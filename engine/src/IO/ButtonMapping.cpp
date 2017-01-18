/* 
 * File:   ButtonMapping.cpp
 * Author: markus
 * 
 * Created on 27. Dezember 2016, 14:13
 */

#include "ButtonMapping.h"
namespace engine {
    namespace IO {
        ButtonMapping::ButtonMapping(GLFWwindow* window) : 
        window(window),
                lastx(0),
                lasty(0)
        {
            devButton empty;
            empty.buttonID = -1000;
            empty.deviceID = -1000;
            this->mapping.set_empty_key(empty);
        }
        
        ButtonMapping::ButtonMapping(const ButtonMapping& mapping) : 
        window(mapping.window),
        mapping(mapping.mapping),
                lastx(0),
                lasty(0)
        {}

        ButtonMapping::~ButtonMapping() {
        }
        
        void ButtonMapping::insertMapping(int deviceID, int buttonID, shared_ptr<Action> msg){
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
        
         vector<shared_ptr<Action>> ButtonMapping::getActions() {
             
             vector<shared_ptr<Action>> actions;
            int prevDevice = -1000;
            int bCount, aCount;
            const unsigned char* buttons;
            const float* axis;
            
            for(auto it : this->mapping) {
                
                if(it.first.deviceID == KEYBOARD) {
                    if(glfwGetKey(this->window, it.first.buttonID) == GLFW_PRESS) {
                        it.second->setDevButton(it.first.deviceID, it.first.buttonID);
                        actions.push_back(it.second);
                    }
                }
                
                else if(it.first.deviceID == MOUSE) {
                    if(it.first.buttonID == -1) {
                        double xpos, ypos;
                        glfwGetCursorPos(this->window, &xpos, &ypos);
                        it.second->setAxes(xpos-lastx, -(ypos-lasty));
                        actions.push_back(it.second);
                        lastx = xpos;
                        lasty = ypos;
                    }
                    else if(glfwGetMouseButton(this->window, it.first.buttonID) == GLFW_PRESS) {
                        it.second->setDevButton(it.first.deviceID, it.first.buttonID);
                        actions.push_back(it.second);
                    }
                }
                
                else if(it.first.deviceID >= 0)
                {
                    if(prevDevice != it.first.deviceID) {
                        buttons = glfwGetJoystickButtons(it.first.deviceID, &bCount);
                        axis = glfwGetJoystickAxes(it.first.deviceID, &aCount);
                    }
                    if(it.first.buttonID < 0 && (it.first.buttonID*2) > -aCount) {
                        double xpos = axis[-(it.first.buttonID)*2-1];
                        double ypos = axis[-(it.first.buttonID)*2];
                        it.second->setAxes(xpos, ypos);
                        actions.push_back(it.second);
                    }
                    else if(it.first.buttonID < bCount && buttons[it.first.buttonID] == GLFW_PRESS) {
                        it.second->setDevButton(it.first.deviceID, it.first.buttonID);
                        actions.push_back(it.second);
                    }
                }
                prevDevice = it.first.deviceID;
            }
            return actions;
        }
    }
}
