#ifndef DEFAULTSHADER_H
#define DEFAULTSHADER_H

#include <string>
#include "../util/vec3.h"

namespace engine {
    namespace renderer {
        using std::string;
        using std::to_string;
        using util::vec3;
        
        class DefaultShader {
        public:
            static string createFlatVertexShader(vec3 color = {1.0f, 0.f, 0.f});
            static string createFlatFragmentShader();
            
            static string createSimpleTextureVertexShader();
            static string createSimpleTextureFragmentShader();
        private:
            DefaultShader() {}  
        };
    }
}

#endif /* DEFAULTSHADER_H */

