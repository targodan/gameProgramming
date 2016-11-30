#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <google/protobuf/message.h>
#include <string>
#include <iostream>

#include "Serializable.h"

namespace engine {
    namespace IO {
        class Serializer {
        public:
            virtual bool serialize(const google::protobuf::Message& msg, std::string& serialized) = 0;
            virtual bool serialize(const google::protobuf::Message& msg, std::ostream& serialized) = 0;
            virtual bool serialize(Serializable& msg, std::string& serialized) {
                return this->serialize(msg.toProtobufMessage(), serialized);
            }
            virtual bool serialize(Serializable& msg, std::ostream& serialized) {
                return this->serialize(msg.toProtobufMessage(), serialized);
            }
            virtual bool deserialize(google::protobuf::Message& msg, const std::string& serialized) = 0;
            virtual bool deserialize(google::protobuf::Message& msg, std::istream& serialized) = 0;
            virtual bool deserialize(Serializable& msg, const std::string& serialized) {
                bool ret = this->deserialize(msg.fromProtobufMessage(), serialized);
                msg.afterProtobufMessageUpdate();
                return ret;
            }
            virtual bool deserialize(Serializable& msg, std::istream& serialized) {
                bool ret = this->deserialize(msg.fromProtobufMessage(), serialized);
                msg.afterProtobufMessageUpdate();
                return ret;
            }
        };
    }
}

#endif /* SERIALIZER_H */

