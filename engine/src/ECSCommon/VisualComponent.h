#ifndef VISUALCOMPONENT_H
#define VISUALCOMPONENT_H

#include "../ECS/Component.h"
#include "../renderer/Mesh.h"
#include "../renderer/Material.h"

namespace engine {
    namespace ECSCommon {
        using engine::ECS::componentId_t;
        using engine::renderer::Mesh;
        using engine::renderer::Material;
        
        class VisualComponent : public engine::ECS::Component {
        private:
            static componentId_t typeId;
            
        protected:
            Mesh mesh;
            Material material;
            
        public:
            VisualComponent();
            VisualComponent(const Mesh& mesh, const Material& material);
            VisualComponent(const VisualComponent& orig) = delete;
            virtual ~VisualComponent();
            
            void setMesh(const Mesh& mesh);
            const Mesh& getMesh() const;
            Mesh& getMesh();
            
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

