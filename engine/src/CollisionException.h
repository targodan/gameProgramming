#ifndef COLLISIONEXCEPTION_H
#define COLLISIONEXCEPTION_H

#include <cstdarg>
#include <memory>

#include "Exception.h"

namespace engine {
    class CollisionException : public Exception {
    public:
        CollisionException(const char* what, ...) {
            va_list list;
            va_start(list, what);
            this->initFromList(what, list);
            va_end(list);
        }
        CollisionException(const char* what, va_list list) {
            this->initFromList(what, list);
        }
        CollisionException(const CollisionException& orig) : Exception(orig) {}
        virtual ~CollisionException() {}
    };
}

#endif /* COLLISIONEXCEPTION_H */

