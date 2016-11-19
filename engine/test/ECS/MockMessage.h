#ifndef MOCKMESSAGE_H
#define MOCKMESSAGE_H

#include "ECS/Message.h"

class MockMessage : public engine::ECS::Message {
private:
    size_t payload;
    
public:
    MockMessage(size_t id, size_t payload) : Message(id), payload(payload) {}
    
    size_t getPayload() const {
        return this->payload;
    }
};

#endif /* MOCKMESSAGE_H */

