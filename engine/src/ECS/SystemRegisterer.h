#ifndef SYSTEMREGISTERER_H
#define SYSTEMREGISTERER_H

#include "macros.h"
#include "SystemRegistry.h"

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

#endif /* SYSTEMREGISTERER_H */

