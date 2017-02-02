#ifndef SKYBOX_H
#define SKYBOX_H

#include "VisualObject.h"
#include "VertexArray.h"
#include "EnvironmentTextureType.h"

namespace engine {
    namespace renderer {
        class Skybox : public VisualObject {
        protected:
            EnvironmentTextureType textureType;
            
        public:
            Skybox(const Texture& texture, EnvironmentTextureType texType);
            
            Skybox(const Skybox& orig);
            Skybox(Skybox&& orig);
            Skybox& operator=(const Skybox& right);
            Skybox& operator=(Skybox&& right);
        };
    }
}

#endif /* SKYBOX_H */

