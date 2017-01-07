#ifndef MATERIAL_H
#define MATERIAL_H

#include "ShaderProgram.h"
#include <memory>

namespace engine {
    namespace renderer {
        class Material {
        public:
            Material(std::shared_ptr<ShaderProgram> shader);
            Material(const Material& orig);
            Material(Material&& orig);
            
            Material& operator=(const Material& right);
            Material& operator=(Material&& right);
            
            virtual ~Material();
            void releaseMaterial();
            
            // This needs to be called before drawing the corresponding mesh
            void makeActive() const;
            
            std::shared_ptr<const ShaderProgram> getShader() const;
        private:
            
            // NOTE: I made this a shared_ptr because I couldn't see how to work
            //       with this class otherwise. Why a unique_ptr?
            // Why not?
            std::shared_ptr<ShaderProgram> shader;    
        };
    }
}


#endif /* MATERIAL_H */

