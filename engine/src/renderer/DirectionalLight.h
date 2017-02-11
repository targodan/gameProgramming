#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "LightSource.h"
#include "util/vec3.h"

namespace engine {
    namespace renderer {
        using util::vec3;
        
        class DirectionalLight : public LightSource {
        public:
            DirectionalLight(const vec3& direction);
            DirectionalLight(const vec3& direction, const vec3& diffuseColor);
            DirectionalLight(const vec3& direction, const vec3& diffuseColor, const vec3& ambientColor);
            DirectionalLight(const vec3& direction, const vec3& diffuseColor, const vec3& ambientColor, const vec3& specularColor);
            DirectionalLight(const DirectionalLight& orig);
            DirectionalLight(DirectionalLight&& orig);
            virtual ~DirectionalLight();
            
            const vec3& getDirection() const;
        private:
            vec3 direction; // Direction pointing FROM the light source
        };
    }
}


#endif /* DIRECTIONALLIGHT_H */

