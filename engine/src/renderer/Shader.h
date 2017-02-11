#ifndef SHADER_H
#define SHADER_H

#include "ShaderType.h"
#include "gl/gl_core_3_3.h"
#include "util/readFile.h"
#include "util/Map.h"
#include "util/vector.h"
#include "GLException.h"
#include <memory>
#include <easylogging++.h>

namespace engine {
    namespace renderer {
        using namespace gl;
        using engine::util::Map;
        using engine::util::readFile;
        using engine::util::vector;
        using std::unique_ptr;
        
        /**
         * Shader is a low-level class that wraps creating and compiling GLSL 
         * shader using the RAII principle (i.e., load and compile shader source code on 
         * creation and release it on deletion)
         */
        class Shader {
        public:
            Shader(std::shared_ptr<std::string> sourceCode, ShaderType type) 
                : type(type), sourceCode(sourceCode) {
                this->initShader();
                this->compileShader();
            }
            Shader(const Shader& orig) 
                : type(orig.type), sourceCode(std::make_shared<std::string>(*(orig.sourceCode))) {
                this->initShader();
                this->compileShader();
            }
            Shader(Shader&& orig) 
                : id(std::move(orig.id)), type(std::move(orig.type)), 
                  compiled(std::move(orig.compiled)), sourceCode(std::move(orig.sourceCode)) {}
            Shader& operator=(const Shader& right) {
                this->type = right.type;
                this->sourceCode = std::make_unique<std::string>(*(right.sourceCode));
                
                this->initShader();
                this->compileShader();
                
                return *this;
            }
            Shader& operator=(Shader&& right) {
                this->id = std::move(right.id);
                this->type = std::move(right.type);
                this->compiled = std::move(right.compiled);
                this->sourceCode = std::move(right.sourceCode);
                
                return *this;
            }
            ~Shader() {
                // this->releaseShader();
            }
            
            void releaseShader() {
                glDeleteShader(this->id);
            }
            
            GLuint getID() const {
                return this->id;
            }
            bool isCompiled() const {
                return this->compiled;
            }
            
        private:
            void initShader() {
                this->id = glCreateShader(this->type);
                
                auto sourceCodeCString = sourceCode.get()->c_str();
                glShaderSource(this->id, 1, &sourceCodeCString, NULL);
            }
            void compileShader() {
                glCompileShader(this->id);  
                    
                GLint shaderError;
                glGetShaderiv(this->id, GL_COMPILE_STATUS, &shaderError);
                if(shaderError != GL_TRUE) {
                    GLsizei length = 0;
                    glGetShaderiv(this->id, GL_INFO_LOG_LENGTH, &length);
                    if(length>=1) {
                        GLchar* pInfo = new char[length+1];
                        glGetShaderInfoLog(this->id, length, &length, pInfo);
                        
                        std::cout << "Compile error in " << ShaderTypeToString(this->type) << ": " << std::string(pInfo) << std::endl;
                    }
                    throw GLException("Shader compile error. :(");
                }
                
                this->compiled = true;
            }
            
            GLuint id;
            ShaderType type;
            bool compiled = false;
            
            // vector<std::string> uniformVariables;
            // vector<std::string> inputVariables;
            
            std::shared_ptr<std::string> sourceCode;
           
        };
    }
}

#endif /* SHADER_H */

