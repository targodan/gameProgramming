#ifndef MATERIAL_H
#define MATERIAL_H

#include "ShaderProgram.h"
#include <memory>

namespace engine {
    namespace renderer {
        class Material {
        public:
            Material(std::unique_ptr<ShaderProgram> shader);
            Material(const Material& orig) = delete;
            virtual ~Material();
        private:
            std::unique_ptr<ShaderProgram> shader;    
        };
    }
}


#endif /* MATERIAL_H */

