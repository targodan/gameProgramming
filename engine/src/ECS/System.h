#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <stdlib.h>

namespace engine {
    namespace ECS {
        typedef std::size_t systemTypeId;
        
        class System {
        public:
            System() {}
            System(const System& orig) {}
            virtual ~System() {}

            virtual systemTypeId getSystemTypeId() const = 0;
            virtual std::string getSystemName() const = 0;
            virtual std::string toString() const {
                return this->getSystemName();
            }
        };
    }
}

#endif /* SYSTEM_H */

