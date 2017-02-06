#include "LightingSystem.h"

#include "../ECS/SystemRegisterer.h"
#include "PlacementComponent.h"
#include "LightingComponent.h"
#include "VisualComponent.h"
#include "renderer/TextureType.h"
#include "renderer/ShaderProgram.h"

#include "../WTFException.h"

#include <easylogging++.h>

namespace engine {
    namespace ECSCommon {
        using renderer::TextureType;
        using renderer::ShaderProgram;
        using renderer::DefaultShader;
        
        ECS_REGISTER_SYSTEM(LightingSystem);
        
        systemId_t LightingSystem::systemId = 0;
            
        void LightingSystem::run(EntityManager& em, float deltaTimeSeconds) {
            unsigned int nPointLights = 0;
            bool directionalLight = false;
            for(auto itLight = em.begin({LightingComponent::getComponentTypeId()}); itLight != em.end(); ++itLight) {
                auto& light = itLight[0]->to<LightingComponent>();
                try{
                    auto& placement = em.getEntity(light.getEntityId()).getComponent<PlacementComponent>(); // found a point light
                    nPointLights++;
                } catch(...) {
                    directionalLight = true;
                }
            }
            
            int textures;
            bool applyNormalMapping = this->mapping == Mapping::NONE ? false : true;
            
            for(auto itVisual = em.begin({VisualComponent::getComponentTypeId()}); itVisual != em.end(); ++itVisual) {
                auto& visual = itVisual->to<VisualComponent>();
                auto& material = visual.getVisualObject().getMaterial();
                
                bool hasSpecularTexture = material.hasSpecularTexture();
                bool hasDiffuseTexture = material.hasDiffuseTexture();
                bool hasNormalTexture = material.hasNormalTexture();
                
                if(hasSpecularTexture) {
                    textures = TextureType::SPECULAR;
                }

                if(hasDiffuseTexture) {
                    textures |= TextureType::DIFFUSE;
                }

                if(applyNormalMapping && hasNormalTexture) {
                    textures |= TextureType::NORMAL;
                }
                
                if(nPointLights != this->nPreviousPointLights) {
                    material.setShader(std::make_shared<ShaderProgram>(ShaderProgram::createShaderProgramFromSource(DefaultShader::createLightingVertexShader(), DefaultShader::createLightingFragmentShader(nPointLights, directionalLight, textures))));
                }
                
                visual.setShaderUniform("shininess", material.getShininess());
                if(!(textures & TextureType::SPECULAR)) {
                    visual.setShaderUniform("specularColor", vec3(material.getColor().specular.getGLColor()));
                }
                if(!(textures & TextureType::DIFFUSE)) {
                    visual.setShaderUniform("diffuseColor", vec3(material.getColor().diffuse.getGLColor()));
                }
                
                unsigned int iLightSource = 0;
                for(auto itLight = em.begin({LightingComponent::getComponentTypeId()}); itLight != em.end(); ++itLight) {
                    auto& lightSource =  itLight->to<LightingComponent>().getLightSource();
                    try{
                        auto& placement = em.getEntity(itLight.getEntityId()).getComponent<PlacementComponent>(); // found a point light
                        
                        std::string uniformLightSource = "pointLightSources[" + std::to_string(iLightSource) + "].";
                        visual.setShaderUniform(uniformLightSource + ".position", placement.getPosition());
                        visual.setShaderUniform(uniformLightSource + ".ambient", lightSource.getAmbient());
                        visual.setShaderUniform(uniformLightSource + ".diffuse", lightSource.getDiffuse());
                        visual.setShaderUniform(uniformLightSource + ".specular", lightSource.getSpecular());
                        visual.setShaderUniform(uniformLightSource + ".linAttenuation", lightSource.getLinAttenuation());
                        visual.setShaderUniform(uniformLightSource + ".quadAttenuation", lightSource.getQuadAttenuation());
                    } catch(...) {}
                }
            }
        }
        
        systemId_t LightingSystem::getSystemTypeId() const {
            return LightingSystem::systemId;
        }
        
        systemId_t LightingSystem::systemTypeId() {
            return LightingSystem::systemId;
        }
        
        void LightingSystem::setSystemTypeId(systemId_t id) {
            LightingSystem::systemId = id;
        }
    }
}