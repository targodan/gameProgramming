/* 
 * File:   ButtonMapping.h
 * Author: markus
 *
 * Created on 27. Dezember 2016, 14:12
 */

#ifndef BUTTONMAPPING_H
#define BUTTONMAPPING_H

#include <GLFW/glfw3.h>
#include <memory>
#include "../util/Map.h"
#include "../ECS/Message.h"
#include "../ECS/MessageHandler.h"

#define GLFW_JOYSTICK_A 0
#define GLFW_JOYSTICK_B 1
#define GLFW_JOYSTICK_X 2
#define GLFW_JOYSTICK_Y 3
#define GLFW_JOYSTICK_SL 4
#define GLFW_JOYSTICK_SR 5
#define GLFW_JOYSTICK_BACK 6
#define GLFW_JOYSTICK_START 7
#define GLFW_JOYSTICK_STICK_L 8
#define GLFW_JOYSTICK_STICK_R 9
#define GLFW_JOYSTICK_UP 10
#define GLFW_JOYSTICK_RIGHT 11
#define GLFW_JOYSTICK_DOWN 12
#define GLFW_JOYSTICK_LEFT 13

namespace engine {
    namespace IO {
        using engine::util::Map;
        using engine::ECS::Message;
        using engine::ECS::MessageHandler;
        using std::shared_ptr;
        
        class ButtonMapping {
        public:
            ButtonMapping(GLFWwindow* window, shared_ptr<MessageHandler> handler, int dev );
            ButtonMapping( const ButtonMapping& orig ) = delete;
            virtual ~ButtonMapping();
            void insertMapping(int buttonId, shared_ptr<Message> msg);
            void deleteMapping(int buttonId);
            void deleteMapping(shared_ptr<Message> msg);
            void queueMessages();
        private:
            GLFWwindow* window;
            shared_ptr<MessageHandler> handler;
            int device;
            Map<int, shared_ptr<Message>> mapping;

        };
    }
}


#endif /* BUTTONMAPPING_H */

