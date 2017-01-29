#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include "Material.h"
#include "Mesh.h"
#include "../util/vector.h"

#include <string>

namespace engine {
    namespace renderer {
        using std::string;
        using util::vector;
        
        class VisualObject {
        public:
            VisualObject();
            VisualObject(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material);
            VisualObject(string pathToModel, string pathToVertexShader = "", string pathToFragmentShader = "");
            
            VisualObject(const VisualObject& orig);
            VisualObject(VisualObject&& orig);
            VisualObject& operator=(const VisualObject& right);
            VisualObject& operator=(VisualObject&& right);
            
            virtual ~VisualObject();
            
            void loadObject();
            void render();
            
            const std::shared_ptr<Mesh>& getMesh() const;
            std::shared_ptr<Mesh>& getMesh();
            
            const std::shared_ptr<Material>& getMaterial() const;
            std::shared_ptr<Material>& getMaterial();
            
            bool isInitialized() const;
        private:
            void init();
            
            bool initialized;
            std::shared_ptr<Mesh> mesh;
            std::shared_ptr<Material> material;
        };
    }
}


#endif /* VISUALOBJECT_H */

