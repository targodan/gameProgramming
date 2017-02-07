#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include "../util/vec3.h"

namespace engine {
    namespace renderer {
        using util::vec3;
        
        
        class LightSource {
        public:
            LightSource();
            LightSource(const vec3& diffuseColor);
            LightSource(const vec3& diffuseColor, const vec3& ambientColor);
            LightSource(const vec3& diffuseColor, const vec3& ambientColor, const vec3& specularColor);
            LightSource(const LightSource& orig);
            LightSource(LightSource&& orig);
            virtual ~LightSource();
            
            const vec3& getDiffuse() const;
            const vec3& getAmbient() const;
            const vec3& getSpecular() const;
            
            float getLinAttenuation() const;
            float getQuadAttenuation() const;
            
            void setAttenuation(float linearAttenuation, float quadraticAttenuation);
        protected:
            /*
             * Comments based on: https://learnopengl.com/#!Lighting/Materials
             */
            vec3 diffuseColor;  // usually bright white ~(0.8, 0.8, 0.8)
            vec3 ambientColor; // usually low value ~(0.2, 0.2, 0.2)
            vec3 specularColor;  // usually brightest white (1,1,1)
            
            float linearAttenuation;
            float quadraticAttenuation;
            
            bool directionalLight;
        };

    }
}

#endif /* LIGHTSOURCE_H */

