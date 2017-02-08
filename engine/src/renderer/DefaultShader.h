#ifndef DEFAULTSHADER_H
#define DEFAULTSHADER_H

#include "TextureType.h"
#include <string>
#include "../util/vec3.h"

namespace engine {
    namespace renderer {
        using std::string;
        using std::to_string;
        using util::vec3;
        
        enum DefaultShaderType {
            FLAT,
            FLAT_INSTANCING,
            SIMPLE_TEXTURE_SHADER,
            LIGHTING
        };
        
        class DefaultShader {
        public:
            static string createFlatVertexShader(vec3 color = {1.0f, 0.f, 0.f});
            static string createFlatFragmentShader();
            
            static string createSimpleTextureVertexShader();
            static string createSimpleTextureFragmentShader();
            
            static string createFlatInstancingVertexShader(vec3 color = {1.0f, 0.f, 0.f});
            static string createFlatInstancingFragmentShader();
            
            static string createLightingVertexShader();
            static string createLightingFragmentShader(unsigned int nPointLights, bool directionalLight, int textures);

            static string createTextureInstancingVertexShader();
            static string createTextureInstancingFragmentShader();
        private:
            DefaultShader() {}  
        };
    }
}

#endif /* DEFAULTSHADER_H */

