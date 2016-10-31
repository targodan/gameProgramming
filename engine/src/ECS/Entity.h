#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <memory>
#include <sstream>

using std::string;
using std::shared_ptr;

namespace engine {
    namespace ECS {
        typedef unsigned int id_t;
        
        class Entity {
        private:
            static id_t nextId;

            id_t id;
            shared_ptr<string> name;

        public:
            Entity(const string& name);
            Entity(const Entity& orig) = default;
            virtual ~Entity() = default;
            
            id_t getId() const;
            shared_ptr<const string> getName() const;
            
            string toString() const;
            
            friend std::ostream& operator<<(std::ostream& os, const Entity& e);
        };
    }
}

#endif /* ENTITY_H */

