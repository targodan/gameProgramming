#include "Entity.h"

#include <iomanip>

namespace engine {
    namespace ECS {
        Entity::Entity(entityId_t id, EntityManager* em, const std::string& name)
                : id(id), em(em), name(name) {}
        
        entityId_t Entity::getId() const {
            return this->id;
        }
        
        const std::string& Entity::getName() const {
            return this->name;
        }
            
        Component& Entity::getComponent(componentId_t compId) {
            return *this->em->getComponentOfEntity(this->id, compId);
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