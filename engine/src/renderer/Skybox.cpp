#include "Skybox.h"

#include "gl/gl_core_3_3.h"

// Code taken from https://www.opengl.org/discussion_boards/showthread.php/179068-Compute-fragment-s-ray-direction-(world-s-coordinates)

const char* EQUIRECTANGULAR_VERTEX_SHADER = 
    "#version 330\n"
    "\n"
    "layout (location = 0) in vec2 position;\n"
    "uniform mat4 projectionMatrix;\n"
    "uniform mat4 viewMatrix;\n"
    "uniform mat4 modelMatrix;\n"
    "out vec3 rayDirection;\n"
    "\n"
    "void main() {\n"
    "    vec4 reverseVec;\n"
    "    mat4 projectionMatrixInverse = inverse(projectionMatrix);"
    "    mat4 modelViewMatrixInverse = inverse(modelMatrix * viewMatrix);"
    "\n"
    "    /* inverse perspective projection */\n"
    "    reverseVec = vec4(position.xy, 0.0, 1.0);\n"
    "    reverseVec = projectionMatrixInverse * reverseVec;\n"
    "\n"
    "    /* inverse modelview, without translation */\n"
    "    reverseVec.w = 0.0;\n"
    "    reverseVec = modelViewMatrixInverse * reverseVec;\n"
    "\n"
    "    /* send */\n"
    "    rayDirection = vec3(reverseVec);\n"
    "    gl_Position = vec4(position.xy, 0.0, 1.0);\n"
    "}";
const char* EQUIRECTANGULAR_FRAGMENT_SHADER = 
    "#version 330\n"
    "\n"
    "uniform sampler2D equiTex;\n"
    "in vec3 rayDirection;\n"
    "\n"
    "const float PI = 3.14159265358979323846264;\n"
    "\n"
    "void main() {\n"
    "    vec3 normalizedDirection;\n"
    "    vec2 polarDirection;\n"
    "\n"
    "    /* T computation */\n"
    "    normalizedDirection  = normalize(rayDirection);\n"
    "    polarDirection.t = acos(normalizedDirection.y)/PI;\n"
    "\n"
    "    /* S computation */\n"
    "    vec3 rayDirection_noY = vec3(rayDirection.x, 0.0, rayDirection.z);\n"
    "    normalizedDirection  = normalize(rayDirection_noY);\n"
    "\n"
    "    if(normalizedDirection.x >= 0) {\n"
    "        polarDirection.s = acos(-normalizedDirection.z)/(2*PI);\n"
    "    } else {    \n"
    "        polarDirection.s = (acos(normalizedDirection.z) + PI)/(2*PI);\n"
    "    }\n"
    "    /* color */\n"
    "    gl_FragColor = texture2D(equiTex, polarDirection.st);\n"
    "}";
 
const float* quads = new float[8] {
         1.0f,  1.0f,
         1.0f, -1.0f,
        -1.0f,  1.0f,
        -1.0f, -1.0f
    };

namespace engine {
    namespace renderer {
        Skybox::Skybox(const Texture& texture, EnvironmentTextureType texType)
            : VisualObject(), textureType(texType), vao() {
            if(texType != EnvironmentTextureType::EQUIRECTANGULAR) {
                throw WTFException("Sorry, only equirectangular textures are supported for now. ¯\\_(ツ)_/¯");
            }
            this->vao.attachVBO(std::make_unique<VertexBuffer>(reinterpret_cast<const void*>(quads), sizeof(float) * 8, 8, DataUsagePattern::STATIC_DRAW));
            this->material = std::make_shared<Material>(
                    std::make_shared<ShaderProgram>(
                        ShaderProgram::createShaderProgramFromSource(
                            EQUIRECTANGULAR_VERTEX_SHADER,
                            EQUIRECTANGULAR_FRAGMENT_SHADER
                        )
                    )
                );
            this->material->attachTexture(texture);
            
            this->initialized = true;
        }
        
        Skybox::Skybox(const Skybox& orig) : VisualObject(), textureType(orig.textureType) {
            this->material = orig.material;
            this->vao.attachVBO(std::make_unique<VertexBuffer>(reinterpret_cast<const void*>(quads), sizeof(float) * 8, 8, DataUsagePattern::STATIC_DRAW));
            this->initialized = true;
        }
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
        
        void Skybox::loadObject() {
            this->vao.loadData();
            this->material->loadTextures();
            
            auto positionIndex = this->material->getShader()->getAttributeLocation("position");
            
            // State VBO attributes
            VertexAttribute positionAttrib {positionIndex, 8, 
                    DataType::FLOAT, 0, sizeof(float) * 2, 
                    0};

            vector<VertexAttribute> attribs;
            if(positionIndex != -1) {
                attribs.push_back(positionAttrib);
            }
            
            for(auto& vbo : this->vao.getVBOs()) {
                if(vbo->getAttributes().empty()) {
                    vbo->setAttributes(attribs);
                }
            }
            
            this->vao.bind();
            this->vao.setAttributePointers();
            this->vao.unbind();
        }
        void Skybox::render() {
            this->material->makeActive();
            this->vao.bind();
            this->vao.drawArrays(GL_TRIANGLE_STRIP);
            this->vao.unbind();
            this->material->makeInactive();
        }
    }
}