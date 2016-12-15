#ifndef SHADER_H
#define SHADER_H

#include "gl/gl_core_3_3.h"
#include "ShaderType.h"

namespace engine {
    namespace renderer {
        struct Shader {
            GLuint id;
            ShaderType type;
            bool compiled;
            bool linked;
            std::string fileName;
        };
    }
}

#endif /* SHADER_H */

