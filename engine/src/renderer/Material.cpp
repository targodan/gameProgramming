#include "Material.h"

namespace engine {
    namespace renderer {
        Material::Material(std::shared_ptr<ShaderProgram> shader) : shader(shader) {
            
        }
        Material::Material(const Material& orig) : shader(orig.shader) {
        
        }
        Material::Material(Material&& orig) : shader(std::move(orig.shader)) {
            
        } 
        
        Material& Material::operator=(const Material& right) {
            this->shader = right.shader;
            
            return *this;
        }
        Material& Material::operator=(Material&& right) {
            this->shader = std::move(right.shader);
            
            return *this;
        }
        
        Material::~Material() {
            
        }
        
        void Material::releaseMaterial() {
            this->shader->releaseProgram();
        }
        
        void Material::makeActive() const {
            this->shader->useProgram();
        }
        
        std::shared_ptr<const ShaderProgram> Material::getShader() const {
            return this->shader;
        }
    }
}

