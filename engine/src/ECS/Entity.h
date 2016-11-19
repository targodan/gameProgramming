#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <memory>
#include <sstream>

#include "../util/vector.h"
#include "Component.h"
#include "EntityManager.h"
#include "EntityId.h"

using engine::util::vector;
using std::shared_ptr;

namespace engine {
    namespace ECS {
        class Entity {
        private:
            entityId_t id;
            EntityManager* em;
            std::string name;
            
            friend class EntityManager;
            Entity(entityId_t id, EntityManager* em, const std::string& name);
        public:
            Entity(const Entity& orig) = default;
            virtual ~Entity() = default;
            
            entityId_t getId() const;
            const std::string& getName() const;
            
            template<class CompT, typename... Args>
            Entity& addComponent(Args... args) {
#ifdef DEBUG
                if(!std::is_base_of<Component, CompT>::value) {
                    throw WTFException("Only subclasses of engine::ECS::Component can be added to an Entity.");
                }
#endif
                auto comp = std::make_shared<CompT>(args...);
                comp->setEntityId(this->id);
                this->em->addComponent(*this, comp);
                return *this;
            }
            
            std::string toString() const;
            
            bool operator!=(const Entity& right) const {
                return !(*this == right);
            }

            bool operator<(const Entity& right) const {
                return right > * this;
            }

            bool operator<=(const Entity& right) const {
                return !(*this > right);
            }

            bool operator==(const Entity& right) const {
                return this->em == right.em && this->id == right.id;
            }

            bool operator>(const Entity& right) const {
                return this->id > right.id;
            }

            bool operator>=(const Entity& right) const {
                return !(right > *this);
            }
            
            friend std::ostream& operator<<(std::ostream& os, const Entity& e);
        };
    }
}

#endif /* ENTITY_H */

