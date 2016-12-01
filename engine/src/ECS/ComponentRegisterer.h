#ifndef COMPONENTREGISTERER_H
#define COMPONENTREGISTERER_H

#include <google/protobuf/message.h>

#include <string>

#include "macros.h"
#include "ComponentRegistry.h"

#define ECS_REGISTER_COMPONENT(comp) static engine::ECS::ComponentRegisterer<comp> ECS_MAKE_UNIQUE_NAME(comp)(#comp)
#define ECS_REGISTER_COMPONENT_EXPLICITLY_NAMED(comp, name) static engine::ECS::ComponentRegisterer<comp> ECS_MAKE_UNIQUE_NAME(comp)(name)

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
            ComponentRegisterer(std::string name) {
                this->id = ComponentRegistry::getNextId();
                ComponentT::setComponentTypeId(this->id);
                ComponentRegistry::registerComponent(this->id, name, this);
            }
            
            Component* instantiate() override {
                return new ComponentT();
            }
        };
    }
}

#endif /* COMPONENTREGISTERER_H */

