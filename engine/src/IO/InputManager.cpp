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

        InputManager::getInstance() {
            static shared_ptr<InputManager> instance;
            return instance;
        }

        InputManager::registerDevices() {
            this->devices.clear();
            for(int devNum = GLFW_JOYSTICK_1; devNum <= GLFW_JOYSTICK_LAST; devNum++) {
                if(glfwJoystickPresent(devNum)) {
                    this->devices.push_back(devNum);
                }
            }
        }
        
        InputManager::getDevices() {
            return this->devices;
        }

        InputManager::pollEvents() {
            glfwPollEvents();
        }
    }
}
