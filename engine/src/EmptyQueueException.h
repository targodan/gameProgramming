#ifndef EMPTYQUEUEEXCEPTION_H
#define EMPTYQUEUEEXCEPTION_H

#include "Exception.h"

namespace engine {
    class EmptyQueueException : public Exception {
    public:
        EmptyQueueException(const char* what, ...) {
            va_list list;
            va_start(list, what);
            this->initFromList(what, list);
            va_end(list);
        }
        EmptyQueueException(const char* what, va_list list) {
            this->initFromList(what, list);
        }
        EmptyQueueException(const EmptyQueueException& orig) : Exception(orig) {}
        virtual ~EmptyQueueException() {}
    };
}

#endif /* EMPTYQUEUEEXCEPTION_H */

