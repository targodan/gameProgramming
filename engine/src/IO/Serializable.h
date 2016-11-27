#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include <google/protobuf/message.h>

namespace engine {
    namespace IO {
        template<class msg_t>
        class Serializable {
        protected:
            virtual google::protobuf::Message& getProtobufMessage() = 0;
            virtual void onProtobufMessageUpdated();
            
        public:
            virtual void fromProtobufMessage(const msg_t& msg) {
                this->getProtobufMessage().CopyFrom(msg);
                this->onProtobufMessageUpdated();
            }
            virtual const google::protobuf::Message& toProtobufMessage() const {
                return const_cast<Serializable*>(this)->getProtobufMessage();
            }
        };
    }
}

#endif /* SERIALIZABLE_H */

