#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <memory>
#include <sstream>
#include <vector>

#include "Component.h"
#include "EntityManager.h"
#include "EntityId.h"

using std::vector;
using std::shared_ptr;

namespace engine {
    namespace ECS {
        class Entity {
        private:
            static entityId_t nextId;

            entityId_t id;
            EntityManager& em;
            std::string name;
            
            friend class EntityManager;
            Entity(EntityManager& em, const std::string& name);
        public:
            Entity(const Entity& orig) = default;
            virtual ~Entity() = default;
            
            entityId_t getId() const;
            const std::string& getName() const;
            
            template<class CompT, typename... Args>
            void addComponent(Args... args) {
#ifdef DEBUG
                if(!std::is_base_of<Component, CompT>::value) {
                    throw WTFException("Only subclasses of engine::ECS::Component can be added to an Entity.");
                }
#endif
                auto comp = std::make_shared<CompT>(args...);
                comp->setEntity(this->id);
                this->em.addComponent(*this, comp);
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
                return this->id == right.id;
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

