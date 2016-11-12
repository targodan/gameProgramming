#include "Entity.h"

#include <iomanip>

namespace engine {
    namespace ECS {
        entityId_t Entity::nextId = 0;

        Entity::Entity(EntityManager& em, const std::string& name)
                : id(Entity::nextId++), em(em), name(name) {}
        
        entityId_t Entity::getId() const {
            return this->id;
        }
        
        const std::string& Entity::getName() const {
            return this->name;
        }
        
        std::string Entity::toString() const {
            std::stringstream ss;
            ss << this->getName() << "#"
                    << std::setw(sizeof(entityId_t) * 2) << std::hex << this->id;
            return ss.str();
        }
        
        std::ostream& operator<<(std::ostream& os, const Entity& e) {
            os << e.toString();
            return os;
        }
    }
}