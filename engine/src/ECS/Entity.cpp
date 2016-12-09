#include "Entity.h"

#include <iomanip>

namespace engine {
    namespace ECS {
        Entity::Entity(entityId_t id, EntityManager* em, const std::string& name)
                : id(id), em(em), name(name) {}
        
        Entity::Entity(const Entity& orig)
                : id(orig.id), em(orig.em), name(orig.name) {}
        
        Entity& Entity::operator=(const Entity& orig) {
            this->em = orig.em;
            this->id = orig.id;
            this->name = orig.name;
            return *this;
        }
        
        Entity& Entity::operator=(Entity&& orig) {
            std::swap(this->em, orig.em);
            std::swap(this->id, orig.id);
            std::swap(this->name, orig.name);
            return *this;
        }
        
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