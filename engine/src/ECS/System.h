#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <stdlib.h>
#include <initializer_list>
#include <iostream>

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
            
            virtual void run(EntityManager& em, float dT) = 0;
            virtual bool isUpdateSystem() const = 0;
            virtual bool isRenderSystem() const = 0;
            
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
            
            friend std::ostream& operator<<(std::ostream& os, const System& sys) {
                os << sys.toString();
                return os;
            }
        };
    }
}

#endif /* SYSTEM_H */

