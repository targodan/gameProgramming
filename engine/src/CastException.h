#ifndef CASTEXCEPTION_H
#define CASTEXCEPTION_H

#include "Exception.h"

namespace engine {
    class CastException : public Exception {
    public:
        CastException(const char* what, ...) {
            va_list list;
            va_start(list, what);
            this->initFromList(what, list);
            va_end(list);
        }
        CastException(const char* what, va_list list) {
            this->initFromList(what, list);
        }
        CastException(const CastException& orig) : Exception(orig) {}
        virtual ~CastException() {}
    };
}

#endif /* CASTEXCEPTION_H */

