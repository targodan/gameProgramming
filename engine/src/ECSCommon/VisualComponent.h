#ifndef VISUALCOMPONENT_H
#define VISUALCOMPONENT_H

#include "../ECS/Component.h"
#include "../renderer/VisualObject.h"

#include <memory>

namespace engine {
    namespace ECSCommon {
        using engine::ECS::componentId_t;
        using engine::renderer::Mesh;
        using engine::renderer::Material;
        using engine::renderer::VisualObject;
        
        class VisualComponent : public engine::ECS::Component {
        private:
            static componentId_t typeId;
            
        protected:
            VisualObject object;
            
        public:
            VisualComponent();
            VisualComponent(const Mesh& mesh, const Material& material);
            VisualComponent(const VisualObject& object);
            VisualComponent(const VisualComponent& orig) = delete;
            virtual ~VisualComponent();
            
            void setVisualObject(const VisualObject& object);
            const VisualObject& getVisualObject() const;
            VisualObject& getVisualObject();
            
            void setMaterial(const Material& mat);
            const Material& getMaterial() const;
            Material& getMaterial();
            
            componentId_t getComponentId() const override;
            std::string getComponentName() const override;
            std::string toString() const override;

            static void setComponentTypeId(componentId_t id);
            static componentId_t getComponentTypeId();
        };
    }
}


#endif /* VISUALCOMPONENT_H */

