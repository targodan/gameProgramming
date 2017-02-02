#include "AddEntityMessage.h"

namespace engine {
    namespace ECS {
        messageId_t AddEntityMessage::messageId = 0;
        bool AddEntityMessage::messageRegistered = false;
        
        void AddEntityMessage::setMessageId(messageId_t id) {
            AddEntityMessage::messageId = id;
        }
        messageId_t AddEntityMessage::getMessageId() {
            return AddEntityMessage::messageId;
        }
        
        void AddEntityMessage::registerMessageName(MessageHandler& handler) {
            if(!AddEntityMessage::messageRegistered) {
                AddEntityMessage::messageId = handler.registerMessageName(MESSAGE_NAME_ADD_ENTITY_MESSAGE);
                
                AddEntityMessage::messageRegistered = true;
            }
        }
    }
}
