#ifndef MESSAGERECEIVER_H
#define MESSAGERECEIVER_H

#include <memory>

#include "Message.h"

namespace engine {
    namespace ECS {
        using std::shared_ptr;
        
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

