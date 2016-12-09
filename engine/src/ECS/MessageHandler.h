#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <string>
#include <deque>
#include <memory>
#include "../util/Set.h"
#include "../util/Map.h"
#include "../util/vector.h"

#include "messageId.h"
#include "Message.h"
#include "MessageReceiver.h"

namespace engine {
    namespace ECS {
        using engine::util::Set;
        using engine::util::Map;
        using engine::util::vector;
        using std::deque;
        using std::shared_ptr;

        class MessageHandler {
        protected:
            messageId_t nextId;
            Set<messageId_t> ids;
            Map<std::string, messageId_t> names;
            deque<shared_ptr<Message>> queue;
            Map<messageId_t, vector<MessageReceiver*>> receivers;

        public:
            MessageHandler();
            MessageHandler(const MessageHandler& orig) = delete;
            ~MessageHandler();

            messageId_t registerMessageName(const std::string& messageName);
            messageId_t lookupMessageId(const std::string& messageName);
            void queueMessage(const shared_ptr<Message>& msg);
            
            bool hasQueuedMessages() const;
            shared_ptr<Message> popMessageFromQueue();
            
            void registerReceiver(messageId_t msgId, MessageReceiver* recver);
            void dispatch(const shared_ptr<Message>& msg);
        };
    }
}

#endif /* MESSAGEHANDLER_H */

