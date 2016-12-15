#include "Material.h"

namespace engine {
    namespace renderer {
        Material::Material(std::unique_ptr<ShaderProgram> shader) : shader(std::move(shader)) {
            
        }

        Material::~Material() {
            
        }
    }
}

