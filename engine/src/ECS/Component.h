#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include <stdlib.h>
#include <memory>

#include "EntityId.h"

using std::shared_ptr;

namespace engine {
    namespace ECS {
        typedef std::size_t componentId_t;
        
        class Entity;
        
        class Component {
        protected:
            entityId_t entity;

            friend class Entity;
            friend class EntityManager;
            void setEntity(const entityId_t& id) {
                this->entity = id;
            }
        public:
            Component() {}
            Component(const Component& orig) = delete;
            virtual ~Component() {}

            virtual componentId_t getComponentId() const = 0;
            virtual std::string getComponentName() const = 0;
            virtual std::string toString() const = 0;
        };
    }
}

#endif /* COMPONENT_H */

