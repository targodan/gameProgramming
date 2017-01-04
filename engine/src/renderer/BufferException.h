#ifndef BUFFEREXCEPTION_H
#define BUFFEREXCEPTION_H

#include "../Exception.h"

namespace engine {
    class BufferException : public Exception {
    public:
        BufferException(const char* what, ...) {
            va_list list;
            va_start(list, what);
            this->initFromList(what, list);
            va_end(list);
        }
        BufferException(const char* what, va_list list) {
            this->initFromList(what, list);
        }
        BufferException(const BufferException& orig) : Exception(orig) {}
        virtual ~BufferException() {}
    };
}

#endif /* BUFFEREXCEPTION_H */

