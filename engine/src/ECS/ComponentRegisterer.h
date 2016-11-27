#ifndef COMPONENTREGISTERER_H
#define COMPONENTREGISTERER_H

#include <google/protobuf/message.h>

#include "macros.h"
#include "ComponentRegistry.h"

#define ECS_REGISTER_COMPONENT(comp) static engine::ECS::ComponentRegisterer<comp> ECS_MAKE_UNIQUE_NAME(comp)

namespace engine {
    namespace ECS {
        class ComponentInstantiator {
        public:
            virtual Component* instantiate() = 0;
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
    }
}

#endif /* COMPONENTREGISTERER_H */

