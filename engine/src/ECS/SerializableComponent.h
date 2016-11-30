#ifndef SERIALIZABLECOMPONENT_H
#define SERIALIZABLECOMPONENT_H

#include "Component.h"
#include "../IO/Serializable.h"

namespace engine {
    namespace ECS {
        class SerializableComponent : public engine::IO::Serializable, public Component {};
    }
}

#endif /* SERIALIZABLECOMPONENT_H */

