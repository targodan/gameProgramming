#ifndef COMPONENTREGISTERER_H
#define COMPONENTREGISTERER_H

#include "ComponentRegistry.h"

#define ECS_CONCAT(s1, s2) s1 ## s2
#define ECS_MAKE_UNIQUE_NAME(name) ECS_CONCAT(name, __LINE__)
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

