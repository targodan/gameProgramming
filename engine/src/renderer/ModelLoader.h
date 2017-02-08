#ifndef MODELLOADER_H
#define MODELLOADER_H

#include "Mesh.h"
#include "Material.h"
#include "DefaultShader.h"
#include "Mapping.h"

#include <string>
#include <memory>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace engine {
    namespace renderer {
        using std::string;
        using namespace Assimp;
        
        /*
         * This implementation is based on https://learnopengl.com/#!Model-Loading/Model
         */
        class ModelLoader {
        public:
            ModelLoader(string pathToModel, Mapping mapping, bool lighting);
            ModelLoader(const ModelLoader& orig) = delete;
            ModelLoader(ModelLoader&& orig) = delete;
            ~ModelLoader() {}
            
            std::shared_ptr<Mesh> mesh;
            std::shared_ptr<Material> material;
        private:
            void processNode(aiNode* node, const aiScene* scene);
            aiMesh* findAssimpMesh(aiNode* node, const aiScene* scene); // Temporary; used until multi-mesh-models are supported
            std::shared_ptr<Mesh> createMeshObject(aiMesh* mesh, const aiScene* scene);
            std::shared_ptr<Material> createMaterialObject(aiMesh* mesh, const aiScene* scene);
            vector<Texture> createTexturesOfType(aiMaterial* assimpMaterial, aiTextureType type);
            
            string modelDirectory;
            
            Mapping mapping;
            bool lighting;
        };
    }
}

#endif /* MODELLOADER_H */
