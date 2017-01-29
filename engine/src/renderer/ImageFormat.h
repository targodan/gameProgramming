#ifndef IMAGEFORMAT_H
#define IMAGEFORMAT_H

#include "gl/gl_core_3_3.h"

namespace engine {
    namespace renderer {
        enum ImageFormat {
            ALPHA = GL_ALPHA,
            RGB = GL_RGB,
            RGBA = GL_RGBA,
            RGBA8 = GL_RGBA8,
            SRGB = GL_SRGB,
            SRGB_ALPHA = GL_SRGB_ALPHA,
            SRGB8 = GL_SRGB8,
            SRGB8_ALPHA8 = GL_SRGB8_ALPHA8,
            COMPRESSED_SRGB_ALPHA = GL_COMPRESSED_SRGB_ALPHA
        };
    }
}

#endif /* IMAGEFORMAT_H */

