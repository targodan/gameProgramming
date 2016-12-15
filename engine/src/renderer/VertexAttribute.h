#ifndef VERTEXATTRIBUTE_H
#define VERTEXATTRIBUTE_H

#include "gl/gl_core_3_3.h"
#include "DataType.h"

namespace engine {
    namespace renderer {
        struct VertexAttribute {
            GLuint index;
            GLint size;
            DataType type;
            GLboolean normalized;
            GLsizei stride;
            const GLvoid* offset;
        };
    }
}

#endif /* VERTEXATTRIBUTE_H */

