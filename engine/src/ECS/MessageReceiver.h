#ifndef MESSAGERECEIVER_H
#define MESSAGERECEIVER_H

#include <memory>

using std::shared_ptr;

#include "Message.h"

namespace engine {
    namespace ECS {
        class MessageReceiver {
        public:
            MessageReceiver() {}
            MessageReceiver(const MessageReceiver& orig) = delete;
            virtual ~MessageReceiver() {}

            virtual void receive(shared_ptr<Message> msg) = 0;
        };
    }
}

#endif /* MESSAGERECEIVER_H */

