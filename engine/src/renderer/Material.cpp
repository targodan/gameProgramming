#include "Material.h"
#include "TextureUnit.h"
#include "../WTFException.h"
#include <easylogging++.h>

#include "gl/gl_core_3_3.h"

namespace engine {
    namespace renderer {
        int Material::globalID = 0;
        
        Material::Material(float shininess, bool renderAsWireframe) 
            : renderAsWireframe(false), shininess(shininess), id(++Material::globalID) {
            
            // This is necessary for now as no information about the global lighting setup is available yet 
            this->shader = nullptr; 
        }
        
        Material::Material(vec3 color, float shininess, bool renderAsWireframe) 
            : renderAsWireframe(renderAsWireframe), shininess(shininess), id(++Material::globalID) {
            this->color = color;
            
            // This is necessary for now as no information about the global lighting setup is available yet 
            this->shader = nullptr; 
        }
        Material::Material(MaterialColor color, float shininess, bool renderAsWireframe) 
            : renderAsWireframe(renderAsWireframe), shininess(shininess), color(color), id(++Material::globalID) {
            
            // This is necessary for now as no information about the global lighting setup is available yet 
            this->shader = nullptr; 
        }
        Material::Material(std::shared_ptr<ShaderProgram> shader, bool renderAsWireframe) 
            : renderAsWireframe(renderAsWireframe), id(++Material::globalID) {
            this->setShader(shader);
        }
        Material::Material(const Material& orig) : textures(orig.textures), 
                renderAsWireframe(orig.renderAsWireframe), allTexturesLoaded(orig.allTexturesLoaded), 
                lighting(orig.lighting), attachedNewShader(true), shininess(orig.shininess), color(orig.color), id(++Material::globalID) {
            this->setShader(std::make_shared<ShaderProgram>(*orig.shader));
        }
        Material::Material(Material&& orig) : shader(std::move(orig.shader)), textures(std::move(orig.textures)), 
                renderAsWireframe(std::move(orig.renderAsWireframe)), active(std::move(orig.active)), allTexturesLoaded(std::move(orig.allTexturesLoaded)), 
                lighting(std::move(orig.lighting)), attachedNewShader(std::move(orig.attachedNewShader)), shininess(std::move(orig.shininess)), color(std::move(orig.color)) {
            
        } 
        
        Material& Material::operator=(const Material& right) {
            this->setShader(std::make_shared<ShaderProgram>(*right.shader));
            this->textures = right.textures;
            this->renderAsWireframe = right.renderAsWireframe;
            this->allTexturesLoaded = right.allTexturesLoaded;
            this->lighting = right.lighting;
            this->attachedNewShader = true;
            this->shininess = right.shininess;
            this->color = right.color;
            this->id = ++Material::globalID;

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
            if(this->shader) {
                this->shader->releaseProgram();
            }
            
            for(auto& texture : this->textures) {
                texture.releaseTexture();
            }
        }
        
        Material& Material::attachTexture(const std::string& pathToTexture, TextureType type) {
            Texture texture = {pathToTexture, type};
            return this->attachTexture(texture);
        }
        Material& Material::attachTexture(const Texture& texture) {
            this->textures.push_back(texture);
            
            this->allTexturesLoaded = false;
            
            return *this;
        }
        void Material::setTextures(const vector<Texture>& textures) {
            this->textures = textures;
            this->allTexturesLoaded = false;
        }
        void Material::replaceTextureOfType(const std::string& pathToTexture, TextureType type) {
            Texture texture = {pathToTexture, type};
            this->replaceTextureOfType(texture, type);
        }
        void Material::replaceTextureOfType(const Texture& texture, TextureType type) {
            for(auto textureIt = this->textures.begin(); textureIt != this->textures.end(); textureIt++) {
                if(textureIt->getType() == type) {
                    textureIt->releaseTexture();
                    this->textures.erase(textureIt);
                    this->textures.push_back(texture);
                    break;
                }
            }
            this->allTexturesLoaded = false;
        }
        void Material::replaceTexturesOfType(const vector<Texture>& textures, TextureType type) {
            vector<vector<Texture>::iterator> texturesToDelete;
            for(auto textureIt = this->textures.begin(); textureIt != this->textures.end(); textureIt++) {
                if(textureIt->getType() == type) {
                    texturesToDelete.push_back(textureIt);
                }
            }
            
            for(auto textureToDelete: texturesToDelete) {
                textureToDelete->releaseTexture();
                this->textures.erase(textureToDelete);
            }
            
            this->textures.insert(this->textures.end(), textures.begin(), textures.end());
            this->allTexturesLoaded = false;
        }
        
        const vector<Texture>& Material::getTextures() const {
            return this->textures;
        }
        bool Material::hasSpecularTexture() const {
            for(auto& texture : this->textures) {
                if(texture.getType() == TextureType::SPECULAR) {
                    return true;
                }
            }
            
            return false;
        }
        bool Material::hasNormalTexture() const {
            for(auto& texture : this->textures) {
                if(texture.getType() == TextureType::NORMAL) {
                    return true;
                }
            }
            
            return false;
        }
        bool Material::hasDiffuseTexture() const {
            for(auto& texture : this->textures) {
                if(texture.getType() == TextureType::DIFFUSE) {
                    return true;
                }
            }
            
            return false;
        }
        
        
        void Material::makeActive() {
            if(!this->shader){
                throw WTFException("Could not activate material: no shader specified.");
            } else if(this->active) {
                LOG(INFO) << "Tried to activate already activated material.";
                return;
            }
            
            if(this->renderAsWireframe) {
                gl::glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            
            this->shader->useProgram();
            
            /*
             * The texture-unit activation is based on: https://learnopengl.com/#!Model-Loading/Mesh
             */
            if(!this->textures.empty()) {
                int specularCount = 0, diffuseCount = 0, normalCount = 0, heightCount = 0;
                for(unsigned int i = 0; i < this->textures.size(); i++) {
                    Texture::activateTextureUnit(TextureUnit::TEXTURE0+i);

                    std::string uniformName;
                    switch(this->textures[i].getType()) {
                        case TextureType::SPECULAR:
                            uniformName = "specularTexture" + std::to_string(++specularCount);
                            break;
                        case TextureType::DIFFUSE:
                            uniformName = "diffuseTexture"  + std::to_string(++diffuseCount);
                            break;
                        case TextureType::NORMAL:
                            uniformName = "normalTexture"  + std::to_string(++normalCount);
                            break;
                        case TextureType::HEIGHT:
                            uniformName = "heightTexture"  + std::to_string(++heightCount);
                            break;
                    }
                    
                    this->shader->setUniformi(uniformName, i); // Set texture uniform to current texture unit
                    this->textures[i].bind();
                }
            }
            
            this->active = true;
        }
        void Material::makeInactive() {
            if(this->renderAsWireframe) {
                gl::glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            for(auto& tex : this->textures) {
                tex.unbind();
            }
            
            this->active = false;
        }
        
        void Material::loadTextures() {
            if(this->textures.empty()) {
                throw WTFException("Could not load textures: no textures specified.");
            }
            
            for(auto& texture : textures) {
                if(!texture.isLoaded()) {
                    texture.bind();
                    texture.loadTexture();
                    texture.unbind();
                }
            }
            
            this->allTexturesLoaded = true;
        }
        
        std::shared_ptr<const ShaderProgram> Material::getShader() const {
            return this->shader;
        }
        void Material::setShader(std::shared_ptr<ShaderProgram> shader) {
            if(!shader) {
                throw WTFException("Cannot set shader: given pointer is invalid.");
            } else if(this->active) {
                throw WTFException("Cannot set shader: material currently active. Deactivate first!");
            }
            
            if(this->shader) {
                LOG(INFO) << "Material.setShader: Overwriting current shader.";
                this->shader->releaseProgram();
//                this->attachedNewShader = true;
            }
            this->attachedNewShader = true;
            this->shader = shader;
        }
        
        bool Material::isActive() const {
            return this->active;
        }
        bool Material::areTexturesLoaded() const {
            return this->allTexturesLoaded;
        }
        bool Material::isNewShaderAttached() const {
            return this->attachedNewShader;
        }
        void Material::setNewShaderAttached(bool flag) {
            this->attachedNewShader = flag;
        }
        
        float Material::getShininess() const {
            return this->shininess;
        }
        void Material::setShininess(float shininess) {
            this->shininess = shininess;
        }

        const MaterialColor& Material::getColor() const {
            return this->color;
        }
        void Material::setColor(const MaterialColor& color) {
            this->color = color;
        }
        void Material::setColor(const vec3& color) {
            this->color = color;
        }
        void Material::setAmbient(const vec3& color) {
            this->color.ambient = color;
        }
        void Material::setSpecular(const vec3& color) {
            this->color.specular = color;
        }
        void Material::setDiffuse(const vec3& color) {
            this->color.diffuse = color;
        }
        
        void Material::enableLighting() {
            this->lighting = true;
        }
        void Material::disableLighting() {
            this->lighting = false;
        }
        bool Material::isLightingEnabled() const {
            return this->lighting;
        }
    }
}

