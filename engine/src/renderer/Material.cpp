#include "Material.h"

namespace engine {
    namespace renderer {
        Material::Material(std::shared_ptr<ShaderProgram> shader) : shader(shader) {
            
        }
        
        Material::Material(const Material& orig) : shader(std::move(orig.shader)) {
        
        }

        Material::~Material() {
            
        }
        
        void Material::makeActive() const {
            this->shader->useProgram();
        }
    }
}

