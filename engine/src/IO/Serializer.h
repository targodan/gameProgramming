#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <google/protobuf/message.h>
#include <string>
#include <iostream>

namespace engine {
    namespace IO {
        class Serializer {
        public:
            virtual bool serialize(const google::protobuf::Message& msg, std::string& serialized) = 0;
            virtual bool deserialize(google::protobuf::Message& msg, const std::string& serialized) = 0;
        };
    }
}

#endif /* SERIALIZER_H */

