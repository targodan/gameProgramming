#include "Material.h"
#include "TextureUnit.h"
#include "../WTFException.h"

#include "gl/gl_core_3_3.h"

namespace engine {
    namespace renderer {
        Material::Material(std::shared_ptr<ShaderProgram> shader, bool renderAsWireframe) : shader(shader), renderAsWireframe(renderAsWireframe) {
#ifdef DEBUG
            if(!this->shader) {
                throw WTFException("Could not create material: shader pointer invalid.");
            }
#endif 
            
            this->loadedTextures.set_empty_key("");
        }
        Material::Material(const Material& orig) : shader(orig.shader), textures(orig.textures), renderAsWireframe(orig.renderAsWireframe) {
            
        }
        Material::Material(Material&& orig) : shader(std::move(orig.shader)), textures(std::move(orig.textures)), renderAsWireframe(orig.renderAsWireframe) {
            
        } 
        
        Material& Material::operator=(const Material& right) {
            this->shader = right.shader;
            this->textures = right.textures;
            
            return *this;
        }
        Material& Material::operator=(Material&& right) {
            this->shader = std::move(right.shader);
            this->textures = std::move(right.textures);
            
            return *this;
        }
        
        Material::~Material() {
            
        }
        
        void Material::releaseMaterial() {
            this->shader->releaseProgram();
            for(auto texture : this->textures) {
                texture.releaseTexture();
            }
        }
        
        Material& Material::attachTexture(const std::string& pathToTexture) {
            // Insert path as key with default value 'false'
            auto result = this->loadedTextures.insert(std::make_pair(pathToTexture, false));
            if(!result.second) {
                // TODO: LOG WARNING Get texture from another material
                return *this;
            }
            
            Texture texture = {pathToTexture};
            this->textures.push_back(texture);
            this->loadedTextures[pathToTexture] = true;
            
            return *this;
        }
        
        void Material::makeActive() {
            if(this->renderAsWireframe) {
                gl::glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            
            this->shader->useProgram();
            
            if(!this->textures.empty()) {
                int specularCnt = 0;
                int diffuseCnt = 0;
                for(unsigned int i = 0; i < this->textures.size(); i++) {
                    Texture::activateTextureUnit(TextureUnit::TEXTURE0+i);

                    std::string uniformName;
                    if(textures[i].isSpecular()) {
                        uniformName = "specularTexture" + std::to_string(++specularCnt);
                    } else {
                        uniformName = "diffuseTexture"  + std::to_string(++diffuseCnt);
                    }

                    this->shader->setUniform(uniformName, (GLint) i); // Set texture uniform to current texture unit
                    textures[i].bind();
                }
                // Texture::activateTextureUnit(TextureUnit::TEXTURE0);
            }
        }
        
        void Material::makeInactive() {
            if(this->renderAsWireframe) {
                gl::glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
        }
        
        void Material::loadTextures() {
            if(this->textures.empty()) {
                throw WTFException("Could not load textures: no textures specified.");
            }
            
            for(auto texture : textures) {
                texture.bind();
                texture.loadTexture();
                texture.unbind();
            }
        }
        
        std::shared_ptr<const ShaderProgram> Material::getShader() const {
            return this->shader;
        }
    }
}

