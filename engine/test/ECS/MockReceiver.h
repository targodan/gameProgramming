#ifndef MOCKRECEIVER_H
#define MOCKRECEIVER_H

#include "MockMessage.h"
#include "ECS/MessageReceiver.h"

class MockReceiver : public engine::ECS::MessageReceiver {
private:
    size_t* counter;
    
public:
    MockReceiver(size_t* pointer) : counter(pointer) {}
    
    void receive(shared_ptr<engine::ECS::Message> msg) override {
        auto m = msg->to<MockMessage>();
        *this->counter += m.getPayload();
    }
};

#endif /* MOCKRECEIVER_H */

