#include "MessageHandler.h"

#include "CollisionException.h"
#include "../IllegalArgumentException.h"
#include "../EmptyQueueException.h"

namespace engine {
    namespace ECS {
        MessageHandler::MessageHandler() : nextId(1) {
            // Use initial id 1, so default constructed ids 0 are invalid.
            this->ids.set_empty_key(SIZE_MAX);
            this->names.set_empty_key("");
            this->receivers.set_empty_key(SIZE_MAX);
        }

        MessageHandler::~MessageHandler() {
        }
        
        messageId_t MessageHandler::registerMessage(const std::string& messageName) {
            if(messageName == "") {
                throw IllegalArgumentException("Empty string is not a valid message name.");
            }
            if(this->names.find(messageName) != this->names.end()) {
                throw CollisionException("The message name \"%s\" has already been registered.", messageName.c_str());
            }
            messageId_t id = this->nextId++;
            this->ids.insert(id);
            this->names[messageName] = id;
            return id;
        }
        
        messageId_t MessageHandler::lookupMessageId(const std::string& messageName) {
            auto it = this->names.find(messageName);
            if(it == this->names.end()) {
                throw IllegalArgumentException("Message \"%s\" has not been registered yet.", messageName.c_str());
            }
            return it->second;
        }
        
        void MessageHandler::queueMessage(const shared_ptr<Message>& msg) {
            if(this->ids.find(msg->getId()) == this->ids.end()) {
                throw IllegalArgumentException("The message id %zu is unknown. Did you forget to register?", msg->getId());
            }
            this->queue.push_back(msg);
        }
        
        bool MessageHandler::hasMessages() const {
            return !this->queue.empty();
        }
        
        shared_ptr<Message> MessageHandler::popMessage() {
            if(!this->hasMessages()) {
                throw EmptyQueueException("Popping on an empty queue is a bad idea.");
            }
            shared_ptr<Message> ret = this->queue.front();
            this->queue.pop_front();
            return ret;
        }
        
        void MessageHandler::registerReceiver(messageId_t msgId, MessageReceiver* recver) {
            this->receivers[msgId].push_back(recver);
        }
        
        void MessageHandler::dispatch(const shared_ptr<Message>& msg) {
            if(this->ids.find(msg->getId()) == this->ids.end()) {
                throw IllegalArgumentException("The message id %zu is unknown. Did you forget to register?", msg->getId());
            }
            for(auto& recver : this->receivers[msg->getId()]) {
                recver->receive(msg);
            }
        }
    }
}