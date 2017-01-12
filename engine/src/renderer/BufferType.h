#ifndef BUFFERTYPE_H
#define BUFFERTYPE_H

#include "gl/gl_core_3_3.h"

namespace engine {
    namespace renderer {
        enum BufferType {
            ARRAY_BUFFER = GL_ARRAY_BUFFER,
            ELEMENT_ARRAY_BUFFER = GL_ELEMENT_ARRAY_BUFFER
        };
    }
}

#endif /* BUFFERTYPE_H */

