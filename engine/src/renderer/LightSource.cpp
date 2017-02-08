#include "LightSource.h"

namespace engine {
    namespace renderer {
            LightSource::LightSource() {
                
            }
            LightSource::LightSource(const vec3& diffuseColor, float linearAttenuation, float quadraticAttenuation) 
                : diffuseColor(diffuseColor), ambientColor(0.2, 0.2, 0.2), specularColor(1.0, 1.0, 1.0), linearAttenuation(linearAttenuation), quadraticAttenuation(quadraticAttenuation) {
                
            }
            LightSource::LightSource(const vec3& diffuseColor, const vec3& ambientColor, float linearAttenuation, float quadraticAttenuation) 
                : diffuseColor(diffuseColor), ambientColor(ambientColor), specularColor(1.0, 1.0, 1.0), linearAttenuation(linearAttenuation), quadraticAttenuation(quadraticAttenuation) {
                
            }
            LightSource::LightSource(const vec3& diffuseColor, const vec3& ambientColor, const vec3& specularColor, float linearAttenuation, float quadraticAttenuation) 
                : diffuseColor(diffuseColor), ambientColor(ambientColor), specularColor(specularColor), linearAttenuation(linearAttenuation), quadraticAttenuation(quadraticAttenuation) {
                
            }
            
            LightSource::LightSource(const LightSource& orig) {
                this->diffuseColor = orig.diffuseColor;
                this->ambientColor = orig.ambientColor;
                this->specularColor = orig.specularColor;
                
                this->linearAttenuation = orig.linearAttenuation;
                this->quadraticAttenuation = orig.quadraticAttenuation;
            }
            LightSource::LightSource(LightSource&& orig) {
                this->diffuseColor = std::move(orig.diffuseColor);
                this->ambientColor = std::move(orig.ambientColor);
                this->specularColor = std::move(orig.specularColor);
                
                this->linearAttenuation = std::move(orig.linearAttenuation);
                this->quadraticAttenuation = std::move(orig.quadraticAttenuation);
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
    }
}