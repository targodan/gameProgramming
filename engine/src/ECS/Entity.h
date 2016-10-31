#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <memory>
#include <sstream>

using std::string;
using std::shared_ptr;

namespace engine {
    namespace ECS {
        typedef std::size_t entityId;
        
        class Entity {
        private:
            static entityId nextId;

            entityId id;
            shared_ptr<string> name;

        public:
            Entity(const string& name);
            Entity(const Entity& orig) = default;
            virtual ~Entity() = default;
            
            entityId getId() const;
            shared_ptr<const string> getName() const;
            
            string toString() const;
            
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

