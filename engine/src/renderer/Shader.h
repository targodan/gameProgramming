#ifndef SHADER_H
#define SHADER_H

#include "ShaderType.h"
#include "gl/gl_core_3_3.h"
#include "../util/readFile.h"
#include "../util/Map.h"
#include <memory>

namespace engine {
    namespace renderer {
        using namespace gl;
        using engine::util::Map;
        using engine::util::readFile;
        using std::unique_ptr;
        
        /**
         * Shader is a low-level class that wraps creating and compiling GLSL 
         * shader using the RAII principle (i.e., load and compile shader source code on 
         * creation and release it on deletion)
         */
        class Shader {
        public:
            Shader(std::unique_ptr<std::string> sourceCode, ShaderType type) 
                : type(type), compiled(false), sourceCode(std::move(sourceCode)) {
                this->initShader();
                this->compileShader();
            }
           
            ~Shader() {
                this->releaseShader();
            }
            
            Shader(const Shader& orig) = delete;
            Shader(Shader&& orig) = delete;
            
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
                    
                // TODO: Check for compile errors

                this->compiled = true;
            }
            void releaseShader() {
                glDeleteShader(this->id);
            }
            
            GLuint id;
            ShaderType type;
            bool compiled;
            const std::unique_ptr<std::string> sourceCode;
           
        };
    }
}

#endif /* SHADER_H */

