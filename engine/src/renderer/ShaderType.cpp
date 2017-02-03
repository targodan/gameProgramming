#include "ShaderType.h"

namespace engine {
    namespace renderer {
        std::string ShaderTypeToString(ShaderType type) {
            switch(type) {
                case ShaderType::FRAGMENT_SHADER:
                    return "FRAGMENT_SHADER";
                case ShaderType::VERTEX_SHADER:
                    return "VERTEX_SHADER";
                case ShaderType::GEOMETRY_SHADER:
                    return "GEOMETRY_SHADER";
                default:
                    return "NO_SHADER";
            }
            return "NO_SHADER";
        }
    }
}