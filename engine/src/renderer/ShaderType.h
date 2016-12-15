#ifndef SHADERTYPE_H
#define SHADERTYPE_H

#include "gl/gl_core_3_3.h"
#include "../util/Map.h"

namespace engine {
    namespace renderer {
        enum ShaderType {
            NO_SHADER = -1,
            VERTEX_SHADER = GL_VERTEX_SHADER,
            FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
            GEOMETRY_SHADER = GL_GEOMETRY_SHADER
        };
    }
}


#endif /* SHADERTYPE_H */

