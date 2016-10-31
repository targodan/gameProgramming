#ifndef WTFEXCEPTION_H
#define WTFEXCEPTION_H

#include "Exception.h"

namespace engine {
    class WTFException : public Exception {
    public:
        WTFException(const char* what) : Exception(what) {}
        virtual ~WTFException() {}
    };
}

#endif /* WTFEXCEPTION_H */

