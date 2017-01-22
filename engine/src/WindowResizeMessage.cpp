#include "WindowResizeMessage.h"

#include "ECS/MessageHandler.h"

namespace engine {
    ECS::messageId_t WindowResizeMessage::id = -1;
        
    void WindowResizeMessage::registerMessageType(ECS::MessageHandler& mh) {
        WindowResizeMessage::id = mh.registerMessageName(WINDOW_RESIZE_MESSAGE_NAME);
    }
    
    ECS::messageId_t WindowResizeMessage::getMessageTypeId() {
        return WindowResizeMessage::id;
    }
}
