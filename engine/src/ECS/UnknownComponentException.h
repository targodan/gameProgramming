#ifndef UNKNOWNCOMPONENTEXCEPTION_H
#define UNKNOWNCOMPONENTEXCEPTION_H

#include <cstdarg>
#include <memory>

#include "../Exception.h"

namespace engine {
    namespace ECS {
        class UnknownComponentException : public Exception {
        public:
            UnknownComponentException(const char* what, ...) {
                va_list list;
                va_start(list, what);
                this->initFromList(what, list);
                va_end(list);
            }
            UnknownComponentException(const char* what, va_list list) {
                this->initFromList(what, list);
            }
            UnknownComponentException(const UnknownComponentException& orig) : Exception(orig) {}
            virtual ~UnknownComponentException() {}
        };
    }
}


#endif /* UNKNOWNCOMPONENTEXCEPTION_H */

