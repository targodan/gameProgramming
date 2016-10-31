#ifndef INVALIDDIMENSIONEXCEPTION_H
#define INVALIDDIMENSIONEXCEPTION_H

#include "../Exception.h"

namespace engine {
    namespace math {
        class InvalidDimensionException : public Exception {
        public:
            InvalidDimensionException(const char* what) : Exception(what) {}
            virtual ~InvalidDimensionException() {}
        };
    }
}

#endif /* INVALIDDIMENSIONEXCEPTION_H */

