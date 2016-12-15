#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "gl/gl_core_3_3.h"
#include "ShaderType.h"
#include "../util/vector.h"
#include "../util/Map.h"
#include "Shader.h"
#include <memory>

namespace engine {
    namespace renderer {
        using namespace gl;
        using namespace util;
        using std::unique_ptr;
        
        class ShaderProgram {
        public:
            ShaderProgram(std::string vertexShaderFile, std::string fragmentShaderFile);
            ShaderProgram(const ShaderProgram& orig) = delete;
            virtual ~ShaderProgram();
            
            void use();
            bool isLinked() const;
        private:
            std::string _getShaderFileExtensionForType(ShaderType type) const;
            
            void createShader(std::string fileName, ShaderType type);
            void compile();
            void link();
            
            bool linked;
            
            GLuint shaderProg;
            Map<ShaderType, Shader*> registeredShaders;
            //Map<ShaderType, std::unique_ptr<Shader>> registeredShaders;
        };
    }
}

#endif /* SHADERPROGRAM_H */

