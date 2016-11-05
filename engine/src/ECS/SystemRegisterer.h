#ifndef SYSTEMREGISTERER_H
#define SYSTEMREGISTERER_H

#include "macros.h"
#include "SystemRegistry.h"

#define ECS_REGISTER_SYSTEM(comp) static engine::ECS::SystemRegisterer<comp> ECS_MAKE_UNIQUE_NAME(comp)

namespace engine {
    namespace ECS {
        template<class SystemT>
        class SystemRegisterer {
        public:
            SystemRegisterer() {
                SystemT::setSystemTypeId(SystemRegistry::getNextId());
            }
        };
    }
}

#endif /* SYSTEMREGISTERER_H */

