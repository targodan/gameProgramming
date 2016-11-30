#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include <google/protobuf/message.h>

namespace engine {
    namespace IO {
        class Serializable {
        public:
            virtual google::protobuf::Message& fromProtobufMessage() = 0;
            virtual void afterProtobufMessageUpdate() = 0;
            virtual const google::protobuf::Message& toProtobufMessage() = 0;
        };
    }
}

#endif /* SERIALIZABLE_H */

