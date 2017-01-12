#ifndef DATAUSAGEPATTERN_H
#define DATAUSAGEPATTERN_H

#include "gl/gl_core_3_3.h"

namespace engine {
    namespace renderer {
        using namespace gl;
        
        /* 
         * Specifies how the buffered data (via a VBO) is used and, depending on
         * that, changes how the graphics card manages the given data.
         * 
         * [Taken from www.learnopengl.com]:
         * GL_STATIC_DRAW: the data will most likely not change at all or very rarely.
         * GL_DYNAMIC_DRAW: the data is likely to change a lot.
         * GL_STREAM_DRAW: the data will change every time it is drawn.
         * 
         */
        enum DataUsagePattern {
            STATIC_DRAW = GL_STATIC_DRAW,
            DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
            STREAM_DRAW = GL_STREAM_DRAW
        };
    }
}

#endif /* DATAUSAGEPATTERN_H */

