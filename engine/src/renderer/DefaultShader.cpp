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
        
        string DefaultShader::createLightingVertexShader(bool normalMapping) {
            /*
             * Important todo: get viewPos and lightPos as uniforms in VERTEX-SHADER
             * and transform them with the inverse TBN (= transposed TBN) into
             * tangent space -> way faster than doing the other way around in
             * fragment-shader
             */
            
            string inputTangents = "";
            string outputTBN = "";
            string calcTBN = "";
            if(normalMapping) {
                inputTangents =
                        "layout (location = 3) in vec3 tangent;\n"
                        "layout (location = 4) in vec3 bitangent;\n"
                        "\n";
                outputTBN =
                        "out mat3 TBN;\n";
                calcTBN = 
                        "vec3 T = normalize(vec3(modelViewMatrix * vec4(tangent, 0.0)));\n"
                        "vec3 B = normalize(vec3(modelViewMatrix * vec4(bitangent, 0.0)));\n"
                        "vec3 N = normalize(vec3(modelViewMatrix * vec4(normal, 0.0)));\n"
                        "TBN = mat3(T, B, N);\n";
            }
          
            return "#version 330\n"
                    "#extension GL_ARB_explicit_attrib_location : require\n"
                    "#extension GL_ARB_separate_shader_objects : require\n"
                    ""   
                    "layout (location = 0) in vec3 position;\n"
                    "layout (location = 1) in vec3 normal;\n"
                    "layout (location = 2) in vec2 textureCoordinates;\n"
                    + inputTangents +
                    "\n"
                    "uniform mat4 modelMatrix = mat4(vec4(1,0,0,0), vec4(0,1,0,0), vec4(0,0,1,0), vec4(0,0,0,1));\n"
                    "uniform mat4 viewMatrix;\n"
                    "uniform mat4 projectionMatrix;\n"
                    "\n"
                    "out vec3 fragPosition;\n"
                    "out vec3 fragNormal;\n"
                    "out vec2 uv;\n"
                    "out mat4 oModelViewMatrix;\n"
                    + outputTBN +
                    ""
                    "void main() {\n"
                    "   mat4 modelViewMatrix = viewMatrix * modelMatrix;\n"
                    ""
                    "   fragPosition = vec3(modelViewMatrix * vec4(position, 1.0f));\n"
                    "   fragNormal = vec3(inverse(transpose(modelViewMatrix)) * vec4(normal, 0.f));\n"
                    "   uv = textureCoordinates;\n"
                    ""
                    "   gl_Position = projectionMatrix * vec4(fragPosition, 1.0f);\n"
                    ""
                    "   oModelViewMatrix = modelViewMatrix;\n"  
                    + calcTBN +
                    "}\n";
        }
        // Formulae closely oriented on Basic Techniques in Computer Graphics & www.learnopengl.com
        string DefaultShader::createLightingFragmentShader(unsigned int nPointLights, bool directionalLight, int textures, bool normalMapping) {
            if(nPointLights == 0 && !directionalLight) {
                throw WTFException("Cannot create lighting fragment shader for 0 light sources!");
            }
            
            string inTBN = "";
            string getNormal = "";
            string normalInput = "";
            if(normalMapping) {
                inTBN = "in mat3 TBN;\n";
                getNormal = "     vec3 unitNorm = normalize(TBN * normalize((texture(normalTexture1, uv).rgb)*2.0 - 1.0));\n";
                normalInput = "uniform sampler2D normalTexture1;\n";
            } else {
                getNormal = "     vec3 unitNorm = normalize(fragNormal);\n";
                normalInput = "in vec3 fragNormal;\n";
            }
            
            
            bool useDiffuseTexture = (textures & TextureType::DIFFUSE) != 0;
            bool useSpecularTexture = (textures & TextureType::SPECULAR) != 0;
            
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
                    "     vec3 diffuseColor = light.diffuse * calcDiffuseTerm(unitNormal, unitLightDirection) * " + diffuseValue + ";\n"
                    "     vec3 specularColor = light.specular * calcSpecularTerm(unitNormal, unitLightDirection, unitViewDirection) * " + specularValue + ";\n"
                    "     return ambientColor + calcAttenuationFactor(length(fragPosition - vec3(oModelViewMatrix * vec4(light.position, 1.f))), light.linAttenuation, light.quadAttenuation) * (diffuseColor + specularColor);\n"
                    "     // return unitNormal;\n"
                    "     // return "  +diffuseValue+ ";\n"
                    "}\n";
            }
            
            string main = 
                    "void main() {\n"
                    + getNormal +
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
                    + normalInput 
                    + inTBN +
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
        string DefaultShader::createTextureInstancingFragmentShader(float opacity) {
            return "#version 330\n"
                    "in vec2 uv;\n"
                    ""
                    "out vec4 fragmentColor;\n"
                    ""
                    "uniform sampler2D diffuseTexture1;\n"
                    ""
                    "void main() {\n"
                    "    vec4 color = vec4(texture(diffuseTexture1, uv));\n"
                    "    color.a *= " + std::to_string(opacity) + ";"
                    "    fragmentColor = color;"
                    "}";
        }
    }
}
