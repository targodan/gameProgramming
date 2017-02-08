#include "DefaultShader.h"
#include "../WTFException.h"

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

        string DefaultShader::createLightingVertexShader() {
            return "#version 330\n"
                    "#extension GL_ARB_explicit_attrib_location : require\n"
                    "#extension GL_ARB_separate_shader_objects : require\n"
                    ""   
                    "layout (location = 0) in vec3 position;\n"
                    "layout (location = 1) in vec3 normal;\n"
                    "layout (location = 2) in vec2 textureCoordinates;\n"
                    ""
                    "uniform mat4 modelMatrix = mat4(vec4(1,0,0,0), vec4(0,1,0,0), vec4(0,0,1,0), vec4(0,0,0,1));\n"
                    "uniform mat4 viewMatrix;\n"
                    "uniform mat4 projectionMatrix;\n"
                    ""
                    "out vec3 fragPosition;\n"
                    "out vec3 fragNormal;\n"
                    "out vec2 uv;\n"
                    "out mat4 oModelViewMatrix;\n"
                    ""
                    "void main() {\n"
                    "   mat4 modelViewMatrix = viewMatrix * modelMatrix;\n"
                    ""
                    "   fragPosition = vec3(modelViewMatrix * vec4(position, 1.0f));\n"
                    "   fragNormal = normalize(vec3(inverse(transpose(modelViewMatrix)) * vec4(normal, 0.f)));\n"
                    "   uv = textureCoordinates;\n"
                    ""
                    "   gl_Position = projectionMatrix * vec4(fragPosition, 1.0f);\n"
                    ""
                    "   oModelViewMatrix = modelViewMatrix;\n"  
                    "}";
        }
        string DefaultShader::createLightingFragmentShader(unsigned int nPointLights, bool directionalLight, int textures) {
            if(nPointLights == 0 && !directionalLight) {
                throw WTFException("Cannot create lighting fragment shader for 0 light sources!");
            }
            
            bool useDiffuseTexture = textures & TextureType::DIFFUSE;
            bool useSpecularTexture = textures & TextureType::SPECULAR;
            
            string diffuseValue = "";
            string diffuseInput = "uniform ";
            if(useDiffuseTexture) {
                diffuseInput += "sampler2D diffuseTexture1;\n";
                diffuseValue = "texture(diffuseTexture1, uv).rgb";
            } else {
                diffuseInput += "vec3 diffuseColor;\n";
                diffuseValue = "diffuseColor";
            }
            
            string specularValue = "";
            string specularInput = "uniform ";
            if(useSpecularTexture) {
                specularInput += "sampler2D specularTexture1;\n";
                specularValue = "texture(specularTexture1, uv).rgb";
            } else {
                specularInput += "vec3 specularColor;\n";
                specularValue = "specularColor";
            }
            
            string pointLightInput = "";
            if(nPointLights>0) {
                pointLightInput = "uniform PointLight pointLightSources[" + std::to_string(nPointLights) + "];\n";
            }
            
            string directionalLightInput = "";
            if(directionalLight) {
                directionalLightInput = "uniform DirectionalLight directionalLight;\n";
            }
            
            string directionalLightStruct = "";
            if(directionalLight) {
                directionalLightStruct = 
                        "struct DirectionalLight{\n"
                        "    vec3 direction;\n"
                        ""
                        "    vec3 ambient;\n"
                        "    vec3 specular;\n"
                        "    vec3 diffuse;\n"
                        "};\n";
            }
            
            string pointLightStruct = "";
            if(nPointLights > 0) {
                pointLightStruct = 
                        "struct PointLight{\n"
                        "    vec3 position;\n"
                        ""
                        "    vec3 ambient;\n"
                        "    vec3 specular;\n"
                        "    vec3 diffuse;\n"
                        ""
                        "    float linAttenuation;\n"
                        "    float quadAttenuation;\n"
                        "};\n";
            }
            
            string calculateDirectionalLight = "";
            if(directionalLight) {
                calculateDirectionalLight=
                    "vec3 calculateDirectionalLight(DirectionalLight light, vec3 unitNormal, vec3 unitViewDirection){\n"
                    "     vec3 unitLightDirection = normalize(-light.direction);\n"
                    ""
                    "     vec3 ambientColor = light.ambient * " + diffuseValue + ";\n"
                    "     vec3 diffuseColor = light.diffuse * calcDiffuseTerm(unitNormal, unitLightDirection) * " + diffuseValue + ";\n"
                    "     vec3 specularColor = light.specular * calcSpecularTerm(unitNormal, unitLightDirection, unitViewDirection) * " + specularValue + ";\n"
                    "     return ambientColor + diffuseColor + specularColor;\n"
                    "}\n";
            }
            
            string calculatePointLight = "";
            if(nPointLights > 0) {
                calculatePointLight =
                    "vec3 calculatePointLight(PointLight light, vec3 unitNormal, vec3 unitViewDirection){\n"
                    "     vec3 unitLightDirection = normalize(vec3(oModelViewMatrix * vec4(light.position, 1.f)) - fragPosition);\n"
                    ""
                    "     vec3 ambientColor = light.ambient * " + diffuseValue + ";\n"
                    "     vec3 diffuseColor = " + diffuseValue + ";\n"
                    "     vec3 specularColor = light.specular * calcSpecularTerm(unitNormal, unitLightDirection, unitViewDirection) * " + specularValue + ";\n"
                    "     // return ambientColor + calcAttenuationFactor(length(fragPosition - vec3(oModelViewMatrix * vec4(light.position, 1.f))), light.linAttenuation, light.quadAttenuation) * (diffuseColor + specularColor);\n"
                    "     return diffuseColor;\n"
                    "}\n";
            }
            
            string main = 
                    "void main() {\n"
                    "     vec3 unitNorm = normalize(fragNormal);\n"
                    "     vec3 unitViewingDir = normalize(-fragPosition);\n"
                    "\n";
            
            if(directionalLight) {
                main += "     vec3 color = calculateDirectionalLight(directionalLight, unitNorm, unitViewingDir);\n";
            } else {
                main += "     vec3 color = vec3(0.f, 0.f, 0.f);\n";
            }
            main += "     for(int i = 0; i < " + std::to_string(nPointLights) + "; i++)\n"
                    "         color += calculatePointLight(pointLightSources[i], unitNorm, unitViewingDir);\n"
                    "\n"
                    "     fragmentColor = vec4(color, 1.0f);\n"
                    "}\n";
            
              
            return  "#version 330\n"
                    ""
                    + pointLightStruct + directionalLightStruct +
                    ""
                    "in vec3 fragPosition;\n" // in CC
                    "in vec3 fragNormal;\n" // in CC 
                    "in vec2 uv;\n"
                    "\n"
                    "in mat4 oModelViewMatrix;\n"
                    "\n"
                    "uniform float shininess;\n"
                    + diffuseInput + specularInput +
                    "\n"
                    + directionalLightInput + pointLightInput +
                    "\n"
                    "out vec4 fragmentColor;\n"
                    "\n"
                    "\n"
                    "float calcSpecularTerm(vec3 unitNormal, vec3 unitLightDirection, vec3 unitViewDirection){\n"
                    "     vec3 halfwayDirection = normalize(unitLightDirection + unitViewDirection);\n"
                    "     return pow(max(0.0, dot(unitNormal, halfwayDirection)), shininess);\n"
                    "}\n"
                    "float calcDiffuseTerm(vec3 unitNormal, vec3 unitLightDirection){\n"
                    "     return max(0.0, dot(unitNormal, unitLightDirection));\n"
                    "}\n"
                    "float calcAttenuationFactor(float distanceLightToFrag, float linAttenuation, float quadAttenuation){\n"
                    "     return 1.0 / (1.0 + linAttenuation * distanceLightToFrag + quadAttenuation * distanceLightToFrag * distanceLightToFrag);\n"
                    "}\n"
                    ""
                    + calculateDirectionalLight
                    + calculatePointLight
                    + main;
        }
    }
}
