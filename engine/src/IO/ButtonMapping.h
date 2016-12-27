/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

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

namespace engine {
    namespace IO {
        using engine::util::Map;
        using engine::ECS::Message;
        using engine::ECS::MessageHandler;
        
        class ButtonMapping {
        public:
            ButtonMapping(shared_ptr<MessageHandler> handler);
            ButtonMapping(const ButtonMapping& orig) = delete;
            virtual ~ButtonMapping();
            void insertMapping(int buttonId, Message msg);
            void deleteMapping(int buttonId);
            void deleteMapping(Message msg);
            void dispatchMessages();
        private:
            Map<int, Message> Mapping;
            shared_ptr<MessageHandler> handler;

        };
    }
}


#endif /* BUTTONMAPPING_H */

