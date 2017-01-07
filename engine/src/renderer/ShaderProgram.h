#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "Shader.h"
#include "glm/gtc/type_ptr.hpp"
#include "../WTFException.h"

namespace engine {
    namespace renderer {
        using namespace gl;
        using std::unique_ptr;
        using engine::util::Map;
        
        class ShaderProgram {
        public:
            ShaderProgram(std::string vertexShaderFile, std::string fragmentShaderFile) 
                : linked(false) {
                this->registeredShaders.set_empty_key(ShaderType::NO_SHADER);
                this->id = glCreateProgram();

                createShader(vertexShaderFile, ShaderType::VERTEX_SHADER);
                createShader(fragmentShaderFile, ShaderType::FRAGMENT_SHADER);
                
                this->linkProgram();
            }
            ShaderProgram(const ShaderProgram& orig) 
                : linked(orig.linked), id(orig.id), registeredShaders(orig.registeredShaders) {}
            ShaderProgram(ShaderProgram&& orig) 
                : linked(std::move(orig.linked)), id(std::move(orig.id)), registeredShaders(std::move(orig.registeredShaders)) {}
            ~ShaderProgram() {
                // glDeleteProgram(this->id);
            }
            
            void releaseProgram() {
                glDeleteProgram(this->id);
                
                for(auto it : this->registeredShaders) {
                    it.second->releaseShader();
                }
            }
            
            ShaderProgram(const ShaderProgram& orig) = delete;
            ShaderProgram(ShaderProgram&& orig) = delete;
            
            void useProgram() const {
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
                
                GLint linkError;
                glGetProgramiv(this->id, GL_LINK_STATUS, &linkError);
                if(linkError != GL_TRUE) {
                    throw GLException("Shader link error. :(");
                }
                
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

