#ifndef CAMERAEXCEPTION_H
#define CAMERAEXCEPTION_H

#include <cstdarg>
#include <memory>

#include "Exception.h"

namespace engine {
    namespace ECSCommon {
        class CameraException : public Exception {
        public:
            CameraException(const char* what, ...) {
                va_list list;
                va_start(list, what);
                this->initFromList(what, list);
                va_end(list);
            }
            CameraException(const char* what, va_list list) {
                this->initFromList(what, list);
            }
            CameraException(const CameraException& orig) : Exception(orig) {}
            virtual ~CameraException() {}
        };
    }
}

#endif /* CAMERAEXCEPTION_H */

