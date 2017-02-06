#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include "Material.h"
#include "Mesh.h"
#include "../util/vector.h"
#include "Mapping.h"

#include <string>

namespace engine {
    namespace renderer {
        using std::string;
        using util::vector;
        
        class VisualObject {
        public:
            VisualObject();
            VisualObject(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material);
            VisualObject(string pathToModel, Mapping mapping = Mapping::NONE, bool lighting=false);
            
            VisualObject(const VisualObject& orig);
            VisualObject(VisualObject&& orig);
            VisualObject& operator=(const VisualObject& right);
            VisualObject& operator=(VisualObject&& right);
            
            virtual ~VisualObject();
            
            virtual void loadObject();
            virtual void render();
            virtual int getRenderPriority() const { return 0; }
            
            const Mesh& getMesh() const;
            Mesh& getMesh();
            
            const Material& getMaterial() const;
            Material& getMaterial();
            
            bool isInitialized() const;
            bool isLoaded() const;
        protected:
            virtual void init();
            
            bool initialized;
            std::shared_ptr<Mesh> mesh;
            std::shared_ptr<Material> material;
            
            bool loaded = false;
        };
    }
}


#endif /* VISUALOBJECT_H */

