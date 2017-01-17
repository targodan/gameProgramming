/* 
 * File:   ControlsReceiver.h
 * Author: markus
 *
 * Created on 15. Januar 2017, 17:13
 */

#ifndef CONTROLSRECEIVER_H
#define CONTROLSRECEIVER_H

#include "../src/ECS/Message.h"
#include "../src/ECS/MessageReceiver.h"
#include <memory>

namespace demo {
    using engine::ECS::Message;
    using engine::ECS::MessageReceiver;
    using std::shared_ptr;
            
    class ControlsReceiver : public engine::ECS::MessageReceiver {
    public:
        ControlsReceiver();
        ControlsReceiver(const ControlsReceiver& orig) = delete;
        virtual ~ControlsReceiver();
        void receive(shared_ptr<Message> msg) override;
    private:

    };
}

#endif /* CONTROLSRECEIVER_H */

