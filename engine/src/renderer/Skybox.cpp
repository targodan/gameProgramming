#include "Skybox.h"

#include "gl/gl_core_3_3.h"
#include "DefaultShader.h"

// Code taken from https://www.opengl.org/discussion_boards/showthread.php/179068-Compute-fragment-s-ray-direction-(world-s-coordinates)

std::string EQUIRECTANGULAR_VERTEX_SHADER = 
    "#version 330\n"
    "#extension GL_ARB_explicit_attrib_location : require\n"
    "#extension GL_ARB_separate_shader_objects : require\n"
    "\n"
    "layout (location = 0) in vec3 position;\n"
//    "uniform mat4 modelMatrix = mat4(vec4(1,0,0,0), vec4(0,1,0,0), vec4(0,0,1,0), vec4(0,0,0,1));\n"
//    "uniform mat4 viewMatrix;\n"
//    "uniform mat4 projectionMatrix;\n"
    "\n"
//    "out vec3 rayDirection;\n"
    "\n"
    "void main() {\n"
//    "    vec4 reverseVec;\n"
//    "    mat4 projectionMatrixInverse = inverse(projectionMatrix);"
//    "    mat4 modelViewMatrixInverse = inverse(modelMatrix * viewMatrix);"
//    "\n"
//    "    /* inverse perspective projection */\n"
//    "    reverseVec = vec4(position.xy, 0, 1);\n"
//    "    reverseVec = projectionMatrixInverse * reverseVec;\n"
//    "\n"
//    "    /* inverse modelview, without translation */\n"
//    "    reverseVec.w = 0;\n"
//    "    reverseVec = modelViewMatrixInverse * reverseVec;\n"
//    "\n"
//    "    /* send */\n"
//    "    rayDirection = vec3(reverseVec);\n"
    "    gl_Position = vec4(position.xy, 0, 1);\n"
    "}";
std::string EQUIRECTANGULAR_FRAGMENT_SHADER = 
    "#version 330\n"
    "\n"
//    "uniform sampler2D diffuseTexture1;\n"
//    "in vec3 rayDirection;\n"
//    "\n"
//    "const float PI = 3.14159265358979323846264;\n"
    "out vec4 fragmentColor;\n"
    "\n"
    "void main() {\n"
//    "    vec3 normalizedDirection;\n"
//    "    vec2 polarDirection;\n"
//    "\n"
//    "    /* T computation */\n"
//    "    normalizedDirection  = normalize(rayDirection);\n"
//    "    polarDirection.t = acos(normalizedDirection.y)/PI;\n"
//    "\n"
//    "    /* S computation */\n"
//    "    vec3 rayDirection_noY = vec3(rayDirection.x, 0, rayDirection.z);\n"
//    "    normalizedDirection  = normalize(rayDirection_noY);\n"
//    "\n"
//    "    if(normalizedDirection.x >= 0) {\n"
//    "        polarDirection.s = acos(-normalizedDirection.z)/(2*PI);\n"
//    "    } else {\n"
//    "        polarDirection.s = (acos(normalizedDirection.z) + PI)/(2*PI);\n"
//    "    }\n"
//    "    /* color */\n"
//    "    gl_FragColor = texture2D(diffuseTexture1, polarDirection.st);\n"
    "    fragmentColor = vec4(0, 1, 0, 1);\n"
    "}";

namespace engine {
    namespace renderer {
        Skybox::Skybox(const Texture& texture, EnvironmentTextureType texType)
            : VisualObject(), textureType(texType) {
            if(texType != EnvironmentTextureType::EQUIRECTANGULAR) {
                throw WTFException("Sorry, only equirectangular textures are supported for now. ¯\\_(ツ)_/¯");
            }
            
            this->mesh = std::make_shared<Mesh>(
                        vector<Vertex>({
                            Vertex({-1.0f, -1.0f, 0}),
                            Vertex({ 1.0f, -1.0f, 0}),
                            Vertex({-1.0f,  1.0f, 0}),
                            Vertex({ 1.0f,  1.0f, 0})
                        }), vector<GLuint>({
                            0, 1, 2,
                            2, 1, 3
                        })
                    );
            
            this->material = std::make_shared<Material>(
                    std::make_shared<ShaderProgram>(
                        ShaderProgram::createShaderProgramFromSource(
                            EQUIRECTANGULAR_VERTEX_SHADER,
                            EQUIRECTANGULAR_FRAGMENT_SHADER
                        )
                    )
                );
            
            this->init();
        }
        
        Skybox::Skybox(const Skybox& orig) : VisualObject(orig), textureType(orig.textureType) {}
        Skybox::Skybox(Skybox&& orig) : VisualObject(std::move(orig)), textureType(std::move(orig.textureType)) {}
        Skybox& Skybox::operator=(const Skybox& right) {
            Skybox::operator=(right);
            this->textureType = right.textureType;
            return *this;
        }
        Skybox& Skybox::operator=(Skybox&& right) {
            Skybox::operator=(std::move(right));
            std::swap(this->textureType, right.textureType);
            return *this;
        }
    }
}