#ifndef INVALIDDIMENSIONEXCEPTION_H
#define INVALIDDIMENSIONEXCEPTION_H

#include <cstdarg>
#include <memory>

#include "../Exception.h"

namespace engine {
    namespace math {
        class InvalidDimensionException : public Exception {
        public:
            InvalidDimensionException(const char* what, ...) {
                va_list list;
                va_start(list, what);
                this->initFromList(what, list);
                va_end(list);
            }
            InvalidDimensionException(const char* what, va_list list) {
                this->initFromList(what, list);
            }
            InvalidDimensionException(const InvalidDimensionException& orig) : Exception(orig) {}
            virtual ~InvalidDimensionException() {}
        };
    }
}

#endif /* INVALIDDIMENSIONEXCEPTION_H */

