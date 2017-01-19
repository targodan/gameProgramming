/* 
 * File:   InputManager.cpp
 * Author: markus
 * 
 * Created on 23. Dezember 2016, 16:37
 */

#include "InputManager.h"
namespace engine {
    namespace IO {
        InputManager::InputManager() {
        }

        InputManager::~InputManager() {
        }

        shared_ptr<InputManager> InputManager::getInstance() {
            static shared_ptr<InputManager> instance ;
            return instance;
        }

        void InputManager::registerDevices() {
            this->devices.clear();
            for(int devNum = GLFW_JOYSTICK_1; devNum <= GLFW_JOYSTICK_LAST; devNum++) {
                if(glfwJoystickPresent(devNum)) {
                    device dev;
                    dev.deviceID = devNum;
                    dev.name = glfwGetJoystickName(devNum);
                    this->devices.push_back(dev);
                }
            }
        }
        
        vector<device> InputManager::getDevices() {
            return this->devices;
        }
        
        std::string InputManager::getDevicesString() {
            std::string devs = "";
            std::string curr = "";
            
            for(auto it = this->devices.begin(); it != this->devices.end(); it++) {
                curr = ("Nr.: " + it->deviceID) + ("\tName: " + it->name) + "\n";
                devs += curr;
            }
            
            return devs;
        }

        void InputManager::pollEvents() {
            glfwPollEvents();
        }
    }
}
