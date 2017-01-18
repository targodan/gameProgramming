#ifndef DATATYPE_H
#define DATATYPE_H

#include "gl/gl_core_3_3.h"

namespace engine {
    namespace renderer {
        enum DataType {
            BYTE = GL_BYTE,
            UBYTE = GL_UNSIGNED_BYTE,
            SHORT = GL_SHORT,
            USHORT = GL_UNSIGNED_SHORT,
            INT = GL_INT,
            UINT = GL_UNSIGNED_INT,
            HALF_FLOAT = GL_HALF_FLOAT,
            FLOAT = GL_FLOAT,
            DOUBLE = GL_DOUBLE
        };
    }
}

#endif /* DATATYPE_H */

