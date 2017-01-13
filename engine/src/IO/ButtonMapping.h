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
#include "../ECS/ActionMessage.h"
#include "../ECS/messageId.h"
#include "../ECS/MessageHandler.h"

#define KEYBOARD -1
#define MOUSE -2
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
        using engine::ECS::messageId_t;
        using engine::ECS::ActionMessage;
        using engine::ECS::MessageHandler;
        using std::shared_ptr;
        
        class ButtonMapping {
        public:
            ButtonMapping(GLFWwindow* window, MessageHandler& handler);
            ButtonMapping( const ButtonMapping& orig ) = delete;
            virtual ~ButtonMapping();
            void insertMapping(int deviceID, int buttonID, shared_ptr<Message> msg);
            void deleteMapping(int deviceID, int buttonID);
            void deleteMapping(shared_ptr<Message> msg);
            void queueMessages();
            
            
            typedef struct DevButton {
                int deviceID, buttonID;
                bool operator==(const DevButton& btn) const {
                    return (deviceID == btn.deviceID && buttonID == btn.buttonID);
                }
            } devButton;
            
            class MyHasher {
            public:
                size_t operator()(const DevButton& btn) const {
                    auto h = std::hash<size_t>();
                    return h(h(btn.deviceID) + h(btn.buttonID));
                }
            };
        private:
            MessageHandler& handler;
            GLFWwindow* window;
            Map<devButton, shared_ptr<Message>, MyHasher> mapping;
        };
    }
}


#endif /* BUTTONMAPPING_H */

