#include "LightingSystem.h"

#include "../ECS/SystemRegisterer.h"
#include "PlacementComponent.h"
#include "LightingComponent.h"
#include "VisualComponent.h"
#include "renderer/TextureType.h"
#include "renderer/ShaderProgram.h"
#include "../ECS/Entity.h"

#include "../WTFException.h"
#include "PlacementSystem.h"
#include "renderer/DirectionalLight.h"

#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <easylogging++.h>

namespace engine {
    namespace ECSCommon {
        using namespace renderer;
        
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
#ifdef DEBUG
                    if(dynamic_cast<DirectionalLight*>(&light.getLightSource()) == nullptr) { // yesss, it's dirty
                        throw("Added pointLight without placement component.")
                    }
#endif /*DEBUG*/
                    
                    if(directionalLight == true) {
                        LOG(INFO) << "Only one directional light can be used in a scene. Using the first found one...";
                    } else {
                        directionalLight = true;
                    }
                }
            }
            
            if(nPointLights == 0 && !directionalLight) {
                return;
            }
            
            int textures = 0;
            bool applyNormalMapping = this->mapping == Mapping::NONE ? false : true;
            for(auto itVisual = em.begin({VisualComponent::getComponentTypeId()}); itVisual != em.end(); ++itVisual) {
                auto& visual = itVisual->to<VisualComponent>();
                auto& mesh = visual.getVisualObject().getMesh();
                auto material = visual.getVisualObject().getMesh().getMaterial();
                
                if(!material->isLightingEnabled()) {
                    continue;
                }
                
                bool hasSpecularTexture = material->hasSpecularTexture();
                bool hasDiffuseTexture = material->hasDiffuseTexture();
                bool hasNormalTexture = material->hasNormalTexture();
                
                if(hasSpecularTexture) {
                    textures = TextureType::SPECULAR;
                }
                if(hasDiffuseTexture) {
                    textures |= TextureType::DIFFUSE;
                }
                if(applyNormalMapping) {
                    if(hasNormalTexture) { // Only apply normal mapping if texture is attached
                        textures |= TextureType::NORMAL;
                    } else {
                        applyNormalMapping = false;
                    }
                }
                
                if(nPointLights != this->nPreviousPointLights) {
                    material->setShader(std::make_shared<ShaderProgram>(ShaderProgram::createShaderProgramFromSource(DefaultShader::createLightingVertexShader(applyNormalMapping), DefaultShader::createLightingFragmentShader(nPointLights, directionalLight, textures, applyNormalMapping))));
//                    std::cout << DefaultShader::createLightingVertexShader(applyNormalMapping) << std::endl;
//                    std::cout << DefaultShader::createLightingFragmentShader(nPointLights, directionalLight, textures, applyNormalMapping) << std::endl;
                }
                
                visual.setShaderUniform("shininess", material->getShininess());
                if((textures & TextureType::SPECULAR) == 0) {
                    visual.setShaderUniform("specularColor", vec3(material->getColor().specular.getGLColor()));
                }
                if((textures & TextureType::DIFFUSE) == 0) {
                    visual.setShaderUniform("diffuseColor", vec3(material->getColor().diffuse.getGLColor()));
                }
                
                unsigned int iLightSource = 0;
                for(auto itLight = em.begin({LightingComponent::getComponentTypeId()}); itLight != em.end(); ++itLight) {
                    auto& lightComp = itLight[0]->to<LightingComponent>();
                    auto& lightSource =  lightComp.getLightSource();
                    
                    std::string uniformLightSource = "";
                    try{
                        auto& placement = em.getEntity(lightComp.getEntityId()).getComponent<PlacementComponent>(); // found a point light
                        uniformLightSource = "pointLightSources[" + std::to_string(iLightSource++) + "]";
                        material->getShader()->setUniform(uniformLightSource + ".position", placement.getPosition());
                        material->getShader()->setUniform(uniformLightSource + ".linAttenuation", lightSource.getLinAttenuation());
                        material->getShader()->setUniform(uniformLightSource + ".quadAttenuation", lightSource.getQuadAttenuation());
                    } catch(...) {
                        uniformLightSource = "directionalLightSource";
                        auto dirLight = dynamic_cast<DirectionalLight*>(&lightSource); // :|
                        material->getShader()->setUniform(uniformLightSource + ".direction", dirLight->getDirection());
                    }
                    material->getShader()->setUniform(uniformLightSource + ".ambient", lightSource.getAmbient());
                    material->getShader()->setUniform(uniformLightSource + ".diffuse", lightSource.getDiffuse());
                    material->getShader()->setUniform(uniformLightSource + ".specular", lightSource.getSpecular());
                }
                
//                material->makeActive();
//                material->makeInactive();
                // mesh.setMaterial(std::make_shared<Material>(material));
            }
            
            this->nPreviousPointLights = nPointLights;
        }
        
        Array<systemId_t> LightingSystem::getDependencies() const {
            return {PlacementSystem::systemTypeId()};
        }
        Array<systemId_t> LightingSystem::getOptionalDependencies() const {
            return {};
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