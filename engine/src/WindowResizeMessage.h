#ifndef WINDOWRESIZEMESSAGE_H
#define WINDOWRESIZEMESSAGE_H

#include "ECS/Message.h"
#include "ECS/MessageHandler.h"

#define WINDOW_RESIZE_MESSAGE_NAME "WINDOW_RESIZE_MESSAGE"

namespace engine {
    class WindowResizeMessage : public ECS::Message {
    protected:
        static ECS::messageId_t id;
        
        int newWidth;
        int newHeight;
        
    public:
        WindowResizeMessage(int newWidth, int newHeight) : ECS::Message(WindowResizeMessage::id), newWidth(newWidth), newHeight(newHeight) {}
        WindowResizeMessage(const WindowResizeMessage& msg) : ECS::Message(msg), newWidth(msg.newWidth), newHeight(msg.newHeight) {}
        WindowResizeMessage(WindowResizeMessage&& msg) : ECS::Message(std::move(msg)), newWidth(std::move(msg.newWidth)), newHeight(std::move(msg.newHeight)) {}
        
        int getNewWidth() const {
            return this->newWidth;
        }
        
        int getNewHeight() const {
            return this->newHeight;
        }
        
        static void registerMessageType(ECS::MessageHandler& mh);
        static ECS::messageId_t getMessageTypeId();
    };
}

#endif /* WINDOWRESIZEMESSAGE_H */

