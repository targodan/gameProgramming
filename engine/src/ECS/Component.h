#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include <stdlib.h>

namespace engine {
    namespace ECS {
        typedef std::size_t compTypeId;
        
        class Component {
        public:
            Component() {}
            Component(const Component& orig) {}
            virtual ~Component() {}

            virtual compTypeId getComponentTypeId() const = 0;
            virtual std::string getComponentName() const = 0;
            virtual std::string toString() const = 0;
        };
    }
}

#endif /* COMPONENT_H */

