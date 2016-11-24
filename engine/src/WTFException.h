#ifndef WTFEXCEPTION_H
#define WTFEXCEPTION_H

#include "Exception.h"

namespace engine {
    class WTFException : public Exception {
    public:
        WTFException(const char* what, ...) {
            va_list list;
            va_start(list, what);
            this->initFromList(what, list);
            va_end(list);
        }
        WTFException(const char* what, va_list list) {
            this->initFromList(what, list);
        }
        WTFException(const WTFException& orig) : Exception(orig) {}
        virtual ~WTFException() {}
    };
}

#endif /* WTFEXCEPTION_H */

