#ifndef TEXTURETYPE_H
#define TEXTURETYPE_H

#include "gl/gl_core_3_3.h"

namespace engine {
    namespace renderer {
        enum TextureType {
            TEXTURE_1D = GL_TEXTURE_1D,
            TEXTURE_2D = GL_TEXTURE_2D,
            TEXTURE_3D = GL_TEXTURE_3D
        };
    }
}

#endif /* TEXTURETYPE_H */

