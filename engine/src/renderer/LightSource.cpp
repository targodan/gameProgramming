#include "LightSource.h"
#include <easylogging++.h>

namespace engine {
    namespace renderer {
            LightSource::LightSource() 
                : diffuseColor(0.8, 0.8, 0.8), ambientColor(0.2, 0.2, 0.2), specularColor(1.0, 1.0, 1.0), directionalLight(false) {
                
            }
            LightSource::LightSource(const vec3& diffuseColor) 
                : diffuseColor(diffuseColor), ambientColor(0.2, 0.2, 0.2), specularColor(1.0, 1.0, 1.0), directionalLight(false) {
                
            }
            LightSource::LightSource(const vec3& diffuseColor, const vec3& ambientColor) 
                : diffuseColor(diffuseColor), ambientColor(ambientColor), specularColor(1.0, 1.0, 1.0), directionalLight(false) {
                
            }
            LightSource::LightSource(const vec3& diffuseColor, const vec3& ambientColor, const vec3& specularColor) 
                : diffuseColor(diffuseColor), ambientColor(ambientColor), specularColor(specularColor), directionalLight(false) {
                
            }
            
            LightSource::LightSource(const LightSource& orig) {
                this->diffuseColor = orig.diffuseColor;
                this->ambientColor = orig.ambientColor;
                this->specularColor = orig.specularColor;
                
                this->linearAttenuation = orig.linearAttenuation;
                this->quadraticAttenuation = orig.quadraticAttenuation;
                
                this->directionalLight = orig.directionalLight;
            }
            LightSource::LightSource(LightSource&& orig) {
                this->diffuseColor = std::move(orig.diffuseColor);
                this->ambientColor = std::move(orig.ambientColor);
                this->specularColor = std::move(orig.specularColor);
                
                this->linearAttenuation = std::move(orig.linearAttenuation);
                this->quadraticAttenuation = std::move(orig.quadraticAttenuation);
                
                this->directionalLight = std::move(orig.directionalLight);
            }
            
            LightSource::~LightSource() {
                
            }
            
            const vec3& LightSource::getDiffuse() const {
                return this->diffuseColor;
            }
            const vec3& LightSource::getAmbient() const {
                return this->ambientColor;
            }
            const vec3& LightSource::getSpecular() const {
                return this->specularColor;
            }
            
            float LightSource::getLinAttenuation() const {
                return this->linearAttenuation;
            }
            float LightSource::getQuadAttenuation() const {
                return this->quadraticAttenuation;
            }
            void LightSource::setAttenuation(float linearAttenuation, float quadraticAttenuation) {
                if(this->directionalLight) {
                    LOG(INFO) << "Directional lights do not attenuate. Setting the light source's attenuation will have no effect.";
                }
                
                this->linearAttenuation = linearAttenuation;
                this->quadraticAttenuation = quadraticAttenuation;
            }
    }
}