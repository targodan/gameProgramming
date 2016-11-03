#ifndef INDEXOUTOFBOUNDSEXCEPTION_H
#define INDEXOUTOFBOUNDSEXCEPTION_H

#include "../Exception.h"

namespace engine {
    namespace util {
        class IndexOutOfBoundsException : public Exception {
        public:
            IndexOutOfBoundsException(const char* what, ...) {
                va_list list;
                va_start(list, what);
                this->initFromList(what, list);
                va_end(list);
            }
            IndexOutOfBoundsException(const char* what, va_list list) {
                this->initFromList(what, list);
            }
            IndexOutOfBoundsException(const IndexOutOfBoundsException& orig) : Exception(orig) {}
            virtual ~IndexOutOfBoundsException() {}
        };
    }
}

#endif /* INDEXOUTOFBOUNDSEXCEPTION_H */

