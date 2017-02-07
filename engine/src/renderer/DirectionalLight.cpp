#include "DirectionalLight.h"

namespace engine {
    namespace renderer {
        DirectionalLight::DirectionalLight(const vec3& direction)
            : direction(direction) {}
        DirectionalLight::DirectionalLight(const vec3& direction, const vec3& diffuseColor)
            : LightSource(diffuseColor), direction(direction) {}
        DirectionalLight::DirectionalLight(const vec3& direction, const vec3& diffuseColor, const vec3& ambientColor) 
            : LightSource(diffuseColor, ambientColor), direction(direction) {}
        DirectionalLight::DirectionalLight(const vec3& direction, const vec3& diffuseColor, const vec3& ambientColor, const vec3& specularColor)
            : LightSource(diffuseColor, ambientColor, specularColor), direction(direction) {}
        DirectionalLight::DirectionalLight(const DirectionalLight& orig) {
            this->diffuseColor = orig.diffuseColor;
            this->ambientColor = orig.ambientColor;
            this->specularColor = orig.specularColor;

            this->linearAttenuation = orig.linearAttenuation;
            this->quadraticAttenuation = orig.quadraticAttenuation;

            this->direction = orig.direction;
        }
        DirectionalLight::DirectionalLight(DirectionalLight&& orig) {
            this->diffuseColor = std::move(orig.diffuseColor);
            this->ambientColor = std::move(orig.ambientColor);
            this->specularColor = std::move(orig.specularColor);

            this->linearAttenuation = std::move(orig.linearAttenuation);
            this->quadraticAttenuation = std::move(orig.quadraticAttenuation);
            
            this->direction = std::move(orig.direction);
        }
        DirectionalLight::~DirectionalLight() {}

        const vec3& DirectionalLight::getDirection() const {
            return this->direction;
        }
    }
}


