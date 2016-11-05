#ifndef DEPENDENCYEXCEPTION_H
#define DEPENDENCYEXCEPTION_H

#include <cstdarg>
#include <memory>

#include "../Exception.h"

namespace engine {
    namespace ECS {
        class DependencyException : public Exception {
        public:
            DependencyException(const char* what, ...) {
                va_list list;
                va_start(list, what);
                this->initFromList(what, list);
                va_end(list);
            }
            DependencyException(const char* what, va_list list) {
                this->initFromList(what, list);
            }
            DependencyException(const DependencyException& orig) : Exception(orig) {}
            virtual ~DependencyException() {}
        };
    }
}

#endif /* DEPENDENCYEXCEPTION_H */

