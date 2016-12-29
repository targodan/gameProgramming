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
            virtual ~Material();
        private:
            
            // NOTE: I made this a shared_ptr because I couldn't see how to work
            //       with this class otherwise. Why a unique_ptr?
            std::shared_ptr<ShaderProgram> shader;    
        };
    }
}


#endif /* MATERIAL_H */

