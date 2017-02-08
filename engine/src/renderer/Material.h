#ifndef MATERIAL_H
#define MATERIAL_H

#include "ShaderProgram.h"
#include "DefaultShader.h"
#include "Texture.h"
#include "Color.h"
#include "../util/Map.h"
#include <vector>
#include <memory>

namespace engine {
    namespace renderer {
        using util::Map;
        
        struct MaterialColor {
            Color ambient;
            Color diffuse;
            Color specular;
            
            MaterialColor& operator=(const Color& color) {
                this->ambient = color;
                this->diffuse = color;
                this->specular = Color{0.8f, 0.8f, 0.8f};
                
                return *this;
            }
            MaterialColor& operator=(const vec3& color) {
                this->ambient = color;
                this->diffuse = color;
                this->specular = Color{0.8f, 0.8f, 0.8f};
                
                return *this;
            }
            MaterialColor& operator=(const vec4& color) {
                this->ambient = color;
                this->diffuse = color;
                this->specular = Color{0.8f, 0.8f, 0.8f, 1.0f};
                
                return *this;
            }
        };
        
        class Material {
        public:
            Material(float shininess = 1.f, bool renderAsWireframe = false);
            Material(vec3 color, float shininess = 1.f, bool renderAsWireframe = false);
            Material(MaterialColor color, float shininess = 1.f, bool renderAsWireframe = false);
            Material(std::shared_ptr<ShaderProgram> shader, bool renderAsWireframe = false);
            Material(const Material& orig);
            Material(Material&& orig);
            
            Material& operator=(const Material& right);
            Material& operator=(Material&& right);
            
            virtual ~Material();
            void releaseMaterial();
            
            Material& attachTexture(const std::string& pathToTexture, TextureType type = TextureType::DIFFUSE);
            Material& attachTexture(const Texture& texture);
            void setTextures(const vector<Texture>& textures);
            void replaceTextureOfType(const std::string& pathToTexture, TextureType type);
            void replaceTextureOfType(const Texture& texture, TextureType type);
            void replaceTexturesOfType(const vector<Texture>& textures, TextureType type);
            
            const vector<Texture>& getTextures() const;
            bool hasSpecularTexture() const;
            bool hasNormalTexture() const;
            bool hasDiffuseTexture() const;
            
            // This needs to be called before drawing the corresponding mesh
            void makeActive();
            void makeInactive();
            void loadTextures();
            
            std::shared_ptr<const ShaderProgram> getShader() const;
            void setShader(std::shared_ptr<ShaderProgram> shader);
            
            float getShininess() const;
            void setShininess(float shininess);
            
            const MaterialColor& getColor() const;
            void setColor(const MaterialColor& color);
            void setColor(const vec3& color);
            
            bool isActive() const;
            bool areTexturesLoaded() const;
            bool isNewShaderAttached() const;
            void setNewShaderAttached(bool flag);
            
            void enableLighting();
            void disableLighting();
            bool isLightingEnabled() const;
        private:
            std::shared_ptr<ShaderProgram> shader;    
            vector<Texture> textures;
            bool renderAsWireframe;
            
            bool active = false;
            bool allTexturesLoaded = false;
            bool lighting = false; // If false, objects with this material won't show lighting effects (for example: skybox)
            bool attachedNewShader = false; 
            
            float shininess;
            MaterialColor color;
        };
    }
}


#endif /* MATERIAL_H */

