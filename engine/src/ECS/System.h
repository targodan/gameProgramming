#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <stdlib.h>
#include <initializer_list>

#include "../util/Array.h"

#include "EntityManager.h"

using engine::util::Array;

namespace engine {
    namespace ECS {
        typedef std::size_t systemId_t;
        
        class System {
        public:
            System() {}
            System(const System& orig) {}
            virtual ~System() {}
            
            virtual void run(EntityManager& em) = 0;
            
            virtual Array<systemId_t> getDependencies() const {
                return {};
            }
            virtual Array<systemId_t> getOptionalDependencies() const {
                return {};
            }

            virtual systemId_t getSystemTypeId() const = 0;
            virtual std::string getSystemName() const = 0;
            virtual std::string toString() const {
                return this->getSystemName();
            }
        };
    }
}

#endif /* SYSTEM_H */

