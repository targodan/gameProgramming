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
            VisualObject(const Mesh& mesh, const Material& material);
            VisualObject(string pathToModel, string pathToVertexShader = "", string pathToFragmentShader = "");
            
            VisualObject(const VisualObject& orig);
            VisualObject(VisualObject&& orig);
            VisualObject& operator=(const VisualObject& right);
            VisualObject& operator=(VisualObject&& right);
            
            virtual ~VisualObject();
            
            void loadObject();
            void render();
            
            const Mesh& getMesh() const;
            Mesh& getMesh();
            const Material& getMaterial() const;
            Material& getMaterial();
            
            bool isInitialized() const;
        private:
            void init();
            
            bool initialized;
            Mesh mesh;
            Material material;
        };
    }
}


#endif /* VISUALOBJECT_H */

