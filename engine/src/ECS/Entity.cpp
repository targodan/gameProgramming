#include "Entity.h"

#include <iomanip>

namespace engine {
    namespace ECS {
        entityId Entity::nextId = 0;

        Entity::Entity(const string& name) : id(Entity::nextId++), name(new string) {
            *this->name = name;
        }
        
        entityId Entity::getId() const {
            return this->id;
        }
        
        shared_ptr<const string> Entity::getName() const {
            return this->name;
        }
        
        string Entity::toString() const {
            std::stringstream ss;
            ss << this->name << "#"
                    << std::setw(sizeof(entityId) * 2) << std::hex << this->id
                    << "{}";
            return ss.str();
        }
        
        std::ostream& operator<<(std::ostream& os, const Entity& e) {
            os << e.toString();
            return os;
        }
    }
}