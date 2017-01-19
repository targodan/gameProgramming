/* 
 * File:   InputManager.h
 * Author: markus
 *
 * Created on 23. Dezember 2016, 16:36
 */

#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <GLFW/glfw3.h>
#include <string>
#include <memory>
#include "../util/vector.h"
#include "Device.h"

namespace engine {
    namespace IO {
        using std::shared_ptr;
        using engine::util::vector;
        
        class InputManager {
        public:
            InputManager(const InputManager& orig) = delete;
            void operator=(InputManager const&) = delete;
            virtual ~InputManager();
            static shared_ptr<InputManager> getInstance();
            void registerDevices();
            vector<device> getDevices();
            void pollEvents();
            std::string getDevicesString();
            
        private:
            InputManager();
            vector<device> devices;
            

        };
    }
}

#endif /* INPUTMANAGER_H */

