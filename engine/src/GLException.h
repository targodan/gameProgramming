#ifndef GLEXCEPTION_H
#define GLEXCEPTION_H

#include "Exception.h"

namespace engine {
    class GLException : public Exception {
    public:
        GLException(const char* what, ...) {
            va_list list;
            va_start(list, what);
            this->initFromList(what, list);
            va_end(list);
        }
        GLException(const char* what, va_list list) {
            this->initFromList(what, list);
        }
        GLException(const GLException& orig) : Exception(orig) {}
        virtual ~GLException() {}
    };
}

#endif /* GLEXCEPTION_H */

