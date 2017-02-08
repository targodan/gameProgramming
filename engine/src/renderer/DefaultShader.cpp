#include "DefaultShader.h"

namespace engine {
    namespace renderer {
        string DefaultShader::createFlatVertexShader(vec3 color) {
            return "#version 330\n"
                    "#extension GL_ARB_explicit_attrib_location : require\n"
                    "#extension GL_ARB_separate_shader_objects : require\n"
                    ""   
                    "layout (location = 0) in vec3 position\n;"
                    ""
                    "uniform mat4 modelMatrix = mat4(vec4(1,0,0,0), vec4(0,1,0,0), vec4(0,0,1,0), vec4(0,0,0,1));\n"
                    "uniform mat4 viewMatrix;\n"
                    "uniform mat4 projectionMatrix;\n"
                    ""
                    "out vec3 vertexColor;\n"
                    ""
                    "void main() {\n"
                    "    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0f);\n"
                    "    vertexColor = vec3(" + to_string(color.x) + "," + to_string(color.y) + "," + to_string(color.z) + ");\n"
                    "}";
        }
        string DefaultShader::createFlatFragmentShader() {
            return "#version 330\n"
                    "in vec3 vertexColor;\n"
                    ""
                    "out vec4 fragmentColor;\n"
                    ""
                    "void main() {\n"
                    "    fragmentColor = vec4(vertexColor, 1.0f);\n"
                    "}";
        }
        
        string DefaultShader::createSimpleTextureVertexShader() {
            return "#version 330\n"
                    "#extension GL_ARB_explicit_attrib_location : require\n"
                    "#extension GL_ARB_separate_shader_objects : require\n"
                    ""
                    "layout (location = 0) in vec3 position;\n"
                    "layout (location = 2) in vec2 textureCoordinate;\n"
                    ""
                    "uniform mat4 modelMatrix = mat4(vec4(1,0,0,0), vec4(0,1,0,0), vec4(0,0,1,0), vec4(0,0,0,1));\n"
                    "uniform mat4 viewMatrix;\n"
                    "uniform mat4 projectionMatrix;\n"
                    ""
                    "out vec2 uv;\n"
                    ""
                    "void main() {\n"
                    "    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0f);\n"
                    "    uv = textureCoordinate;\n"
                    "}";
        }
        string DefaultShader::createSimpleTextureFragmentShader() {
            return "#version 330\n"
                    ""
                    "in vec2 uv;\n"
                    ""
                    "out vec4 fragmentColor;\n"
                    ""
                    "uniform sampler2D diffuseTexture1;\n"
                    ""
                    "void main() {\n"
                    "    fragmentColor = vec4(texture(diffuseTexture1, uv));\n"
                    "}";
        }
        
        string DefaultShader::createFlatInstancingVertexShader(vec3 color) {
            return "#version 330\n"
                    "#extension GL_ARB_explicit_attrib_location : require\n"
                    "#extension GL_ARB_separate_shader_objects : require\n"
                    ""   
                    "layout (location = 0) in vec3 position\n;"
                    "layout (location = 1) in vec3 instancePosition\n;"
                    ""
                    "uniform mat4 modelMatrix = mat4(vec4(1,0,0,0), vec4(0,1,0,0), vec4(0,0,1,0), vec4(0,0,0,1));\n"
                    "uniform mat4 viewMatrix;\n"
                    "uniform mat4 projectionMatrix;\n"
                    ""
                    "out vec3 vertexColor;\n"
                    ""
                    "void main() {\n"
                    "    vec4 positionView = viewMatrix * modelMatrix * vec4(instancePosition, 1.0f)\n;"
                    "    positionView.xyz += position\n;"
                    "    gl_Position = projectionMatrix * positionView;\n"
                    "    vertexColor = vec3(" + to_string(color.x) + "," + to_string(color.y) + "," + to_string(color.z) + ");\n"
                    "}";
        }
        string DefaultShader::createFlatInstancingFragmentShader() {
            return "#version 330\n"
                    "in vec3 vertexColor;\n"
                    ""
                    "out vec4 fragmentColor;\n"
                    ""
                    "void main() {\n"
                    "    fragmentColor = vec4(vertexColor, 1.0f);\n"
                    "}";
        }
        
        string DefaultShader::createTextureInstancingVertexShader() {
            return "#version 330\n"
                    "#extension GL_ARB_explicit_attrib_location : require\n"
                    "#extension GL_ARB_separate_shader_objects : require\n"
                    ""   
                    "layout (location = 0) in vec3 position\n;"
                    "layout (location = 1) in vec3 instancePosition\n;"
                    "layout (location = 2) in vec2 textureCoordinate;\n"
                    ""
                    "uniform mat4 modelMatrix = mat4(vec4(1,0,0,0), vec4(0,1,0,0), vec4(0,0,1,0), vec4(0,0,0,1));\n"
                    "uniform mat4 viewMatrix;\n"
                    "uniform mat4 projectionMatrix;\n"
                    ""
                    "out vec2 uv;\n"
                    ""
                    "void main() {\n"
                    "    vec4 positionView = viewMatrix * modelMatrix * vec4(instancePosition, 1.0f)\n;"
                    "    positionView.xyz += position\n;"
                    "    gl_Position = projectionMatrix * positionView;\n"
                    "    uv = textureCoordinate;\n"
                    "}";
        }
        string DefaultShader::createTextureInstancingFragmentShader() {
            return "#version 330\n"
                    "in vec2 uv;\n"
                    ""
                    "out vec4 fragmentColor;\n"
                    ""
                    "uniform sampler2D diffuseTexture1;\n"
                    ""
                    "void main() {\n"
                    "    fragmentColor = vec4(texture(diffuseTexture1, uv));\n"
                    "    fragmentColor.w = 0.2;"
                    "}";
        }
    }
}
