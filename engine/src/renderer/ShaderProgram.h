#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "Shader.h"
#include "../WTFException.h"

namespace engine {
    namespace renderer {
        using namespace gl;
        using std::unique_ptr;
        using engine::util::Map;
        
        class ShaderProgram {
        public:
            ShaderProgram(std::string vertexShaderFile, std::string fragmentShaderFile) : linked(false) {
                this->registeredShaders.set_empty_key(ShaderType::NO_SHADER);
                this->id = glCreateProgram();

                createShader(vertexShaderFile, ShaderType::VERTEX_SHADER);
                createShader(fragmentShaderFile, ShaderType::FRAGMENT_SHADER);
                
                this->linkProgram();
            }
            
            ~ShaderProgram() {
                glDeleteProgram(this->id);
            }
            
            ShaderProgram(const ShaderProgram& orig) = delete;
            ShaderProgram(ShaderProgram&& orig) = delete;
            
            void useProgram() {
                glUseProgram(this->id);
            }
            
            bool isProgramLinked() const {
                return this->linked;
            }
            
            static Map<ShaderType, std::string> type2FileExtension;
        private:
            void createShader(std::string fileName, ShaderType type) {
                std::string sourceCode = readFile(fileName, _getShaderFileExtensionForType(type));
                
                auto sourceCodePtr = std::make_unique<std::string>(sourceCode);
                Shader* shader = new Shader{std::move(sourceCodePtr), type};
                
                registeredShaders[type] = shader;
            }
            
            void linkProgram() {
                for(auto it : this->registeredShaders) {
                    if(it.second->isCompiled()) {
                        glAttachShader(this->id, it.second->getID());
                    }
                }
            
                glLinkProgram(this->id);
                // TODO: Check for linker errors
                
                for(auto it : this->registeredShaders) {
                    if(it.second->isCompiled()) {
                        glDetachShader(this->id, it.second->getID());
                    }
                }
                this->linked = true;
            }
            
            bool linked;
            GLuint id;
            Map<ShaderType, Shader*> registeredShaders;
            
            std::string _getShaderFileExtensionForType(ShaderType type) const {
                switch(type) {
                    case ShaderType::VERTEX_SHADER:
                        return ".vsh";
                    case ShaderType::FRAGMENT_SHADER:
                        return ".fsh";
                    case ShaderType::GEOMETRY_SHADER:
                        return ".gsh";
                    default:
                        std::string err = "Unexpected ShaderType " + std::to_string(type);
                        throw WTFException(err.c_str());
                }
            }
        };
    }
}

#endif /* SHADERPROGRAM_H */

