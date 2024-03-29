#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <stdlib.h>
#include <initializer_list>
#include <iostream>

#include "../util/Array.h"

#include "EntityManager.h"
#include "SystemType.h"

namespace engine {
    namespace ECS {
        using engine::util::Array;
        typedef std::size_t systemId_t;
        
        class System {
        private:
            SystemType currentRunType;
            
        protected:
            SystemType getCurrentRunType() const { return this->currentRunType; }
            
        public:
            System() {}
            System(const System& orig) {}
            virtual ~System() {}
            
            void setCurrentRunType(SystemType type) { this->currentRunType = type; }
            virtual void run(EntityManager& em, float deltaTimeSeconds) = 0;
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

