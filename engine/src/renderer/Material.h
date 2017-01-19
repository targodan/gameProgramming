#ifndef MATERIAL_H
#define MATERIAL_H

#include "ShaderProgram.h"
#include "Texture.h"
#include <vector>
#include <memory>

namespace engine {
    namespace renderer {
        class Material {
        public:
            Material(std::shared_ptr<ShaderProgram> shader, bool renderAsWireframe = false);
            Material(std::shared_ptr<ShaderProgram> shader, const vector<Texture>& textures, bool renderAsWireframe = false);
            Material(const Material& orig);
            Material(Material&& orig);
            
            Material& operator=(const Material& right);
            Material& operator=(Material&& right);
            
            virtual ~Material();
            void releaseMaterial();
            
            // This needs to be called before drawing the corresponding mesh
            void makeActive();
            void makeInactive();
            void loadTextures();
            
            std::shared_ptr<const ShaderProgram> getShader() const;
        private:
            std::shared_ptr<ShaderProgram> shader;    
            vector<Texture> textures;
            bool renderAsWireframe;
        };
    }
}


#endif /* MATERIAL_H */

