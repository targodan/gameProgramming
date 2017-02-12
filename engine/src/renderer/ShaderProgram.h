#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "Shader.h"
#include "ShaderType.h"
#include "DefaultShader.h"
#include "glm/gtc/type_ptr.hpp"
#include "../WTFException.h"
#include "../util/Map.h"
#include <easylogging++.h>

namespace engine {
    namespace renderer {
        using namespace gl;
        using std::unique_ptr;
        using engine::util::Map;
        
        class ShaderProgram {
        public:
            ShaderProgram(const std::string& vertexShaderFile, const std::string& fragmentShaderFile) {
                this->registeredShaders.set_empty_key(ShaderType::NO_SHADER);
                this->id = glCreateProgram();
                
                createShaderFromFile(vertexShaderFile, ShaderType::VERTEX_SHADER);
                createShaderFromFile(fragmentShaderFile, ShaderType::FRAGMENT_SHADER);
                
                this->linkProgram();
            }
            ShaderProgram(const ShaderProgram& orig) {
                auto shaderMap = orig.getRegisteredShaders();
                Shader* vertexShader = shaderMap[ShaderType::VERTEX_SHADER];
                Shader* fragmentShader = shaderMap[ShaderType::FRAGMENT_SHADER];
                
                this->id = glCreateProgram();
                this->registeredShaders.set_empty_key(ShaderType::NO_SHADER);
                this->registeredShaders[ShaderType::VERTEX_SHADER] = new Shader(*vertexShader);
                this->registeredShaders[ShaderType::FRAGMENT_SHADER] = new Shader(*fragmentShader);
                
                this->linkProgram();
            }
            ShaderProgram(ShaderProgram&& orig) 
                : linked(std::move(orig.linked)), id(std::move(orig.id)), registeredShaders(std::move(orig.registeredShaders)) {}
            ~ShaderProgram() {
                
            }
            
            void releaseProgram() {
                glDeleteProgram(this->id);
                
                for(auto it : this->registeredShaders) {
                    it.second->releaseShader();
                }
            }
            
            /*
             * Note on setUniform via templates: 
             * Idea taken from 'Basic Techniques in Computer Graphics',
             *       assignment08/tools/ShaderProgram.hpp. :)
             */
            template<typename T>
            void setUniform(const std::string& nameInShader, T data) const {
                this->setUniform(this->getUniformLocation(nameInShader), data);
            }
            GLint getUniformLocation(const std::string& name) const {
                return glGetUniformLocation(this->id, name.c_str());
            }
            GLint getAttributeLocation(const std::string& name) const {
                return glGetAttribLocation(this->id, name.c_str());
            }
            void bindAttributeLocation(const std::string& name, GLint index) const {
                glBindAttribLocation(this->id, index, name.c_str());
            }
            
            void useProgram() const {
                glUseProgram(this->id);
            }
            
            bool isProgramLinked() const {
                return this->linked;
            }
            const Map<ShaderType, Shader*>& getRegisteredShaders() const {
                return this->registeredShaders;
            }
            GLuint getID() const {
                return this->id;
            }
            
            static Map<ShaderType, std::string> type2FileExtension;
            
            static ShaderProgram createShaderProgramFromSource(const std::string& vertexShader, const std::string& fragmentShader) {
                ShaderProgram sp;
                
                sp.createShader(vertexShader, ShaderType::VERTEX_SHADER);
                sp.createShader(fragmentShader, ShaderType::FRAGMENT_SHADER);
                
                sp.linkProgram();
                
                return sp;
            }
            void setUniformi(const std::string& nameInShader, GLint data) {
                glUniform1i(this->getUniformLocation(nameInShader), data);
            }
        private:
            ShaderProgram() {
                this->registeredShaders.set_empty_key(ShaderType::NO_SHADER);
                this->id = glCreateProgram();
            }
            
            void setUniform(GLint location, GLint data) {
                glUniform1i(location, data);
            }
            
            // 1-4D floats
            void setUniform(GLint location, GLfloat data) const {
                glUniform1f(location, data);
            }
            void setUniform(GLint location, const glm::vec2& data) const {
                glUniform2fv(location, 1, glm::value_ptr(data));
            }
            void setUniform(GLint location, const glm::vec3& data) const {
                glUniform3fv(location, 1, glm::value_ptr(data));
            }
            void setUniform(GLint location, const glm::vec4& data) const {
                glUniform4fv(location, 1, glm::value_ptr(data));
            }
            
            // float matrices
            void setUniform(GLint location, const glm::mat2& data, GLboolean transpose = GL_FALSE) const {
                glUniformMatrix2fv(location, 1, transpose, glm::value_ptr(data));
            }
            void setUniform(GLint location, const glm::mat3& data, GLboolean transpose = GL_FALSE) const {
                glUniformMatrix3fv(location, 1, transpose, glm::value_ptr(data));
            }
            void setUniform(GLint location, const glm::mat4& data, GLboolean transpose = GL_FALSE) const {
                glUniformMatrix4fv(location, 1, transpose, glm::value_ptr(data));
            }
            
            void createShaderFromFile(std::string fileName, ShaderType type) {
                std::string sourceCode = readFile(fileName, _getShaderFileExtensionForType(type));
                this->createShader(sourceCode, type);
            }
            void createShader(std::string sourceCode, ShaderType type) {
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
                
                /*
                 * Polling linker error based on: 'Basic Techniques in Computer Graphics',
                 *       assignment08/tools/ShaderProgram.hpp. :)
                 */
                GLint linkError;
                glGetProgramiv(this->id, GL_LINK_STATUS, &linkError);
                if(linkError != GL_TRUE) {
                    GLsizei length = 0;
                    glGetProgramiv(this->id, GL_INFO_LOG_LENGTH, &length);
                    if(length>1) {
                        GLchar* pInfo = new char[length+1];
                        glGetProgramInfoLog(this->id, length, &length, pInfo);
                        LOG(ERROR) << "Linker log: " << std::string(pInfo) << std::endl;

                    }
                    
                    throw GLException("Shader link error. :(");
                }
                
                for(auto it : this->registeredShaders) {
                    if(it.second->isCompiled()) {
                        glDetachShader(this->id, it.second->getID());
                    }
                }
                this->linked = true;
            }
            
            bool linked = false;
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

