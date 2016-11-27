#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include <google/protobuf/message.h>

namespace engine {
    namespace IO {
        template<class msg_t>
        class Serializable {
        protected:
            virtual google::protobuf::Message& getProtobufMessage() = 0;
            virtual void afterProtobufMessageUpdate() = 0;
            
        public:
            virtual void fromProtobufMessage(const msg_t& msg) {
                this->getProtobufMessage().CopyFrom(msg);
                this->afterProtobufMessageUpdate();
            }
            virtual const google::protobuf::Message& toProtobufMessage() const {
                return const_cast<Serializable*>(this)->getProtobufMessage();
            }
        };
    }
}

#endif /* SERIALIZABLE_H */

