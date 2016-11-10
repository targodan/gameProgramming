#ifndef COMPONENTREGISTERER_H
#define COMPONENTREGISTERER_H

#include "macros.h"
#include "ComponentRegistry.h"

#define ECS_REGISTER_COMPONENT(comp) static engine::ECS::ComponentRegisterer<comp> ECS_MAKE_UNIQUE_NAME(comp)

namespace engine {
    namespace ECS {
        template<class ComponentT>
        class ComponentRegisterer {
        public:
            ComponentRegisterer() {
                ComponentT::setComponentTypeId(ComponentRegistry::getNextId());
            }
        };
    }
}

#endif /* COMPONENTREGISTERER_H */

