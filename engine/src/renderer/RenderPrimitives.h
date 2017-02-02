#ifndef RENDERPRIMITIVES_H
#define RENDERPRIMITIVES_H

#include "gl/gl_core_3_3.h"

namespace engine {
    namespace renderer {
        using namespace gl;
        
        enum RenderPrimitives {
            POINTS = GL_POINTS,
            LINES = GL_LINES,
            LINE_STRIP = GL_LINE_STRIP,
            TRIANGLES = GL_TRIANGLES,
            TRIANGLE_STRIP = GL_TRIANGLE_STRIP
        };
    }
}

#endif /* RENDERPRIMITIVES_H */

