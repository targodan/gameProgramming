#include "ShaderProgram.h"
#include "../WTFException.h"
#include "../util/readFile.h"

namespace engine {
    namespace renderer {
        ShaderProgram::ShaderProgram(std::string vertexShaderFile, std::string fragmentShaderFile) : linked(false) {
            this->registeredShaders.set_empty_key(ShaderType::NO_SHADER);
            this->shaderProg = glCreateProgram();
            
            createShader(vertexShaderFile, ShaderType::VERTEX_SHADER);
            createShader(fragmentShaderFile, ShaderType::FRAGMENT_SHADER);
        }
        
        
        ShaderProgram::~ShaderProgram() {
            for(auto it : this->registeredShaders) {
                delete it.second;
            }
            
            glDeleteProgram(this->shaderProg);
        }
        

        void ShaderProgram::use() {
            glUseProgram(this->shaderProg);
        }
        
        void ShaderProgram::createShader(std::string fileName, ShaderType type) {
            std::string extension = _getShaderFileExtensionForType(type);
            std::string shaderString = readFile(fileName, extension);
            auto shaderCString = shaderString.c_str();
            
            //unique_ptr<Shader> shader = make_unique<Shader>(0, type, false, false, fileName);
            Shader* shader = new Shader{0, type, false, false, fileName};
            
            shader->id = glCreateShader(type);
            glShaderSource(shader->id, 1, &shaderCString, NULL);
            
            this->registeredShaders[ShaderType::VERTEX_SHADER] = shader;
        }
        
        
        void ShaderProgram::compile() {
            for(auto it : this->registeredShaders) {
                if(!it.second->compiled) {
                    glCompileShader(it.second->id);  
                    
                    // TODO: Check for compile errors
                    
                    it.second->compiled = true;
                }
            }
        }
        
        void ShaderProgram::link() {
            for(auto it : this->registeredShaders) {
                if(it.second->compiled && !it.second->linked) {
                    glAttachShader(this->shaderProg, it.second->id);
                }
            }
            
            glLinkProgram(this->shaderProg);
            // TODO: Check for linker errors
            for(auto it : this->registeredShaders) {
                if(it.second->compiled && !it.second->linked) {
                    glDetachShader(this->shaderProg, it.second->id);
                    
                    it.second->linked = true;
                    
                    glDeleteShader(it.second->id);
                }
            }
            this->linked = true;
        }
        
        std::string ShaderProgram::_getShaderFileExtensionForType(ShaderType type) const {
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
        
        bool ShaderProgram::isLinked() const {
            return this->linked;
        }
    }
}


