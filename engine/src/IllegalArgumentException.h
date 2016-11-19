#ifndef ILLEGALARGUMENTEXCEPTION_H
#define ILLEGALARGUMENTEXCEPTION_H

#include "Exception.h"

namespace engine {
    class IllegalArgumentException : public Exception {
    public:
        IllegalArgumentException(const char* what, ...) {
            va_list list;
            va_start(list, what);
            this->initFromList(what, list);
            va_end(list);
        }
        IllegalArgumentException(const char* what, va_list list) {
            this->initFromList(what, list);
        }
        IllegalArgumentException(const IllegalArgumentException& orig) : Exception(orig) {}
        virtual ~IllegalArgumentException() {}
    };
}

#endif /* ILLEGALARGUMENTEXCEPTION_H */

