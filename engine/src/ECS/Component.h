#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>

namespace engine {
    namespace ECS {
        class Component {
        public:
            Component() {}
            Component(const Component& orig) {}
            virtual ~Component() {}

            virtual int getComponentTypeId() const = 0;
            virtual std::string getComponentName() const = 0;
            virtual std::string toString() const = 0;
        };
    }
}

#endif /* COMPONENT_H */

