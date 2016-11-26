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
#ifdef DEBUG
                return dynamic_cast<T&>(*this);
#else
                return static_cast<T&>(*this);
#endif
            }
            
            template<class T>
            const T& to() const {
#ifdef DEBUG
                return dynamic_cast<const T&>(*this);
#else
                return static_cast<const T&>(*this);
#endif
            }
        };
    }
}

#endif /* MESSAGE_H */

