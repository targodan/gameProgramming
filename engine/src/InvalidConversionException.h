#ifndef INVALIDCONVERSIONEXCEPTION_H
#define INVALIDCONVERSIONEXCEPTION_H

#include "Exception.h"

namespace engine {
    class InvalidConversionException : public Exception {
    public:
        InvalidConversionException(const char* what, ...) {
            va_list list;
            va_start(list, what);
            this->initFromList(what, list);
            va_end(list);
        }
        InvalidConversionException(const char* what, va_list list) {
            this->initFromList(what, list);
        }
        InvalidConversionException(const InvalidConversionException& orig) : Exception(orig) {}
        virtual ~InvalidConversionException() {}
    };
}

#endif /* INVALIDCONVERSIONEXCEPTION_H */

