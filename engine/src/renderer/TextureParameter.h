#ifndef TEXTUREPARAMETER_H
#define TEXTUREPARAMETER_H

#include "gl/gl_core_3_3.h"

namespace engine {
    namespace renderer {
        namespace TextureParameter {
            enum Name {
                MIN_FILTER = GL_TEXTURE_MIN_FILTER,
                MAG_FILTER = GL_TEXTURE_MAG_FILTER,
                WRAP_S = GL_TEXTURE_WRAP_S,
                WRAP_T = GL_TEXTURE_WRAP_T
            };
            
            enum FILTERoptions {
                NEAREST = GL_NEAREST,
                LINEAR = GL_LINEAR,
                NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
                LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
                NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
                LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
            };
            
            enum WRAPoptions {
                CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
                CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
                MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
                REPEAT = GL_REPEAT,
            };
        }
        
        
    }
}

#endif /* TEXTUREPARAMETER_H */

