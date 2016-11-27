#ifndef SERIALIZERFACTORY_H
#define SERIALIZERFACTORY_H

#include "Serializer.h"

#include <google/protobuf/message.h>
#include <google/protobuf/util/json_util.h>
#include <memory>

namespace engine {
    namespace IO {
        class SerializerFactory {
        public:
            class BinarySerializer : public Serializer {
            public:
                bool serialize(const google::protobuf::Message& msg, std::string& serialized) override {
                    return msg.SerializeToString(&serialized);
                }
                bool deserialize(google::protobuf::Message& msg, const std::string& serialized) override {
                    return msg.ParseFromString(serialized);
                }
            };
            class HumanReadableSerializer : public Serializer {
            private:
                google::protobuf::util::JsonPrintOptions options;
                
            public:
                HumanReadableSerializer() {
                    // whitespaces and newlines
                    this->options.add_whitespace = true;
                    // otherwise default values are omitted
                    this->options.always_print_primitive_fields = true;
                }
                bool serialize(const google::protobuf::Message& msg, std::string& serialized) override {
                    return google::protobuf::util::MessageToJsonString(msg, &serialized, this->options) == google::protobuf::util::Status::OK;
                }
                bool deserialize(google::protobuf::Message& msg, const std::string& serialized) override {
                    return google::protobuf::util::JsonStringToMessage(serialized, &msg) == google::protobuf::util::Status::OK;
                }
            };
        
        private:
            static std::unique_ptr<BinarySerializer> binSerializer;
            static std::unique_ptr<HumanReadableSerializer> humanSerializer;
            
        public:
            static Serializer& binarySerializer() {
                if(SerializerFactory::binSerializer == nullptr) {
                    SerializerFactory::binSerializer = std::make_unique<BinarySerializer>();
                }
                return *SerializerFactory::binSerializer;
            }
            
            static Serializer& humanReadableSerializer() {
                if(SerializerFactory::humanSerializer == nullptr) {
                    SerializerFactory::humanSerializer = std::make_unique<HumanReadableSerializer>();
                }
                return *SerializerFactory::humanSerializer;
            }
        };
    }
}

#endif /* SERIALIZERFACTORY_H */

