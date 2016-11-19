#ifndef MESSAGE_H
#define MESSAGE_H

#include "messageId.h"

namespace engine {
    namespace ECS {
        class Message {
        protected:
            messageId_t id;
           
        public:
            Message(messageId_t id) : id(id) {}
            Message(const Message& msg) : id(msg.id) {}
            Message(Message&& msg) : id(std::move(msg.id)) {}
            virtual ~Message() {}
            
            virtual void swap(Message& msg) {
                std::swap(this->id, msg.id);
            }
            
            virtual messageId_t getId() const {
                return this->id;
            }
            
            template<class T>
            T& to() {
                return dynamic_cast<T&>(*this);
            }
        };
    }
}

#endif /* MESSAGE_H */

