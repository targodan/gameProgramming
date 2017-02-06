#ifndef TEXTUREDIMENSION_H
#define TEXTUREDIMENSION_H

#include "gl/gl_core_3_3.h"

namespace engine {
    namespace renderer {
        enum TextureDimension {
            TEXTURE_1D = GL_TEXTURE_1D,
            TEXTURE_2D = GL_TEXTURE_2D,
            TEXTURE_3D = GL_TEXTURE_3D
        };
    }
}

#endif /* TEXTUREDIMENSION_H */

