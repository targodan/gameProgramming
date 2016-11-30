#ifndef COMPONENTREGISTERER_H
#define COMPONENTREGISTERER_H

#include <google/protobuf/message.h>

#include "macros.h"
#include "ComponentRegistry.h"

#define ECS_REGISTER_COMPONENT(comp) static engine::ECS::ComponentRegisterer<comp> ECS_MAKE_UNIQUE_NAME(comp)
#define ECS_REGISTER_SERIALIZABLE_COMPONENT(comp, message) static engine::ECS::SerializableComponentRegisterer<comp, message> ECS_MAKE_UNIQUE_NAME(comp)

namespace engine {
    namespace ECS {
        class ComponentInstantiator {
        public:
            virtual Component* instantiate() = 0;
        };
        
        class ProtobufMessageInstantiator {
        public:
            virtual google::protobuf::Message* instantiateProtobufMessage() = 0;
        };
        
        template<class ComponentT>
        class ComponentRegisterer : public ComponentInstantiator {
        protected:
            componentId_t id;
        public:
            ComponentRegisterer() {
                this->id = ComponentRegistry::getNextId();
                ComponentT::setComponentTypeId(this->id);
                ComponentRegistry::registerInstantiator(this->id, this);
            }
            
            Component* instantiate() override {
                return new ComponentT();
            }
        };
        
        template<class ComponentT, class MessageT>
        class SerializableComponentRegisterer : public ComponentRegisterer<ComponentT>, public ProtobufMessageInstantiator {
        public:
            SerializableComponentRegisterer() : ComponentRegisterer<ComponentT>() {
                ComponentRegistry::registerProtobufMessageInstantiator(this->id, this);
            }
            
            google::protobuf::Message* instantiateProtobufMessage() override {
                return new MessageT();
            }
        };
    }
}

#endif /* COMPONENTREGISTERER_H */

