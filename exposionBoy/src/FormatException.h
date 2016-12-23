#ifndef FORMATEXCEPTION_H
#define FORMATEXCEPTION_H

#include <cstdarg>
#include <memory>

#include "engine/Exception.h"

namespace explosionBoy {
    class FormatException : public engine::Exception {
    public:
        FormatException(const char* what, ...) {
            va_list list;
            va_start(list, what);
            this->initFromList(what, list);
            va_end(list);
        }
        FormatException(const char* what, va_list list) {
            this->initFromList(what, list);
        }
        FormatException(const FormatException& orig) : Exception(orig) {}
        virtual ~FormatException() {}
    };
}

#endif /* FORMATEXCEPTION_H */

