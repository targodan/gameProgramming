#ifndef VISUALCOMPONENT_H
#define VISUALCOMPONENT_H

#include "../ECS/Component.h"
#include "../renderer/Mesh.h"
#include "../renderer/Material.h"

#include <memory>

namespace engine {
    namespace ECSCommon {
        using engine::ECS::componentId_t;
        using engine::renderer::Mesh;
        using engine::renderer::Material;
        
        class VisualComponent : public engine::ECS::Component {
        private:
            static componentId_t typeId;
            
            // TODO: This shouldn't be here, but I didn't come up with a better solution right now :(
            void _combineMeshAndMaterial(); 
        protected:
            std::shared_ptr<Mesh> mesh;
            Material material;
            
        public:
            VisualComponent();
            VisualComponent(const std::shared_ptr<Mesh>& mesh, const Material& material);
            VisualComponent(const VisualComponent& orig) = delete;
            virtual ~VisualComponent();
            
            void setMesh(const std::shared_ptr<Mesh>& mesh);
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

