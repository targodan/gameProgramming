#ifndef MATERIAL_H
#define MATERIAL_H

#include "ShaderProgram.h"
#include "Texture.h"
#include "../util/Map.h"
#include <vector>
#include <memory>

namespace engine {
    namespace renderer {
        using util::Map;
        
        class Material {
        public:
            Material();
            Material(std::shared_ptr<ShaderProgram> shader, bool renderAsWireframe = false);
            Material(const Material& orig);
            Material(Material&& orig);
            
            Material& operator=(const Material& right);
            Material& operator=(Material&& right);
            
            virtual ~Material();
            void releaseMaterial();
            
            Material& attachTexture(const std::string& pathToTexture);
            Material& attachTexture(Texture& texture);
            void setTextures(const vector<Texture>& textures);
            const vector<Texture>& getTextures() const;
            
            // This needs to be called before drawing the corresponding mesh
            void makeActive();
            void makeInactive();
            void loadTextures();
            
            std::shared_ptr<const ShaderProgram> getShader() const;
        private:
            std::shared_ptr<ShaderProgram> shader;    
            vector<Texture> textures;
            bool renderAsWireframe;
            
            bool texturesLoaded;
            Map<std::string, bool> importedTextures; // Holds paths to textures that have already been loaded into RAM
        };
    }
}


#endif /* MATERIAL_H */

