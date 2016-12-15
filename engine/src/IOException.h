#ifndef IOEXCEPTION_H
#define IOEXCEPTION_H

#include "Exception.h"

namespace engine {
    namespace util {
        class IOException : public Exception {
        public:
            IOException(const char* what, ...) {
                va_list list;
                va_start(list, what);
                this->initFromList(what, list);
                va_end(list);
            }
            IOException(const char* what, va_list list) {
                this->initFromList(what, list);
            }
            IOException(const IOException& orig) : Exception(orig) {}
            virtual ~IOException() {}
        };
    }
}

#endif /* IOEXCEPTION_H */



