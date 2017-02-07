#ifndef LIGHTINGCOMPONENT_H
#define LIGHTINGCOMPONENT_H

#include "../ECS/Component.h"
#include "../renderer/LightSource.h"

namespace engine {
    namespace ECSCommon {
        using namespace engine::ECS;
        using renderer::LightSource;
        
        class LightingComponent : public Component {
        private:
            static componentId_t typeId;
            
        protected:
            LightSource lightSource;
            
        public:
            LightingComponent() {}
            LightingComponent(const LightSource& lightSource) : lightSource(lightSource) {}
            LightingComponent(const LightingComponent& orig) = delete;
            virtual ~LightingComponent() {}
            
            const LightSource& getLightSource() const;
            LightSource& getLightSource();
            
            componentId_t getComponentId() const override;
            std::string getComponentName() const override;
            std::string toString() const override;

            static void setComponentTypeId(componentId_t id);
            static componentId_t getComponentTypeId();
        };
    }
}

#endif /* LIGHTINGCOMPONENT_H */