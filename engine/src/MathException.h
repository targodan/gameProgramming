#ifndef MATHEXCEPTION_H
#define MATHEXCEPTION_H

#include "Exception.h"

namespace engine {
    class MathException : public Exception {
    public:
        MathException(const char* what, ...) {
            va_list list;
            va_start(list, what);
            this->initFromList(what, list);
            va_end(list);
        }
        MathException(const char* what, va_list list) {
            this->initFromList(what, list);
        }
        MathException(const MathException& orig) : Exception(orig) {}
        virtual ~MathException() {}
    };
}

#endif /* MATHEXCEPTION_H */

