#ifndef MODELLOADER_H
#define MODELLOADER_H

#include "Mesh.h"
#include "Material.h"

#include <string>
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
            ModelLoader(string pathToModel, string pathToVertexShader = "", string pathToFragmentShader = "");
            ModelLoader(const ModelLoader& orig) = delete;
            ModelLoader(ModelLoader&& orig) = delete;
            ~ModelLoader() {}
            
            Mesh mesh;
            Material material;
        private:
            void processNode(aiNode* node, const aiScene* scene);
            aiMesh* findAssimpMesh(aiNode* node, const aiScene* scene); // Temporary; used until multi-mesh-models are supported
            Mesh createMeshObject(aiMesh* mesh, const aiScene* scene);
            Material createMaterialObject(aiMesh* mesh, const aiScene* scene);
            vector<Texture> createTexturesOfType(aiMaterial* assimpMaterial, aiTextureType type);
            
            string pathToVertexShader;
            string pathToFragmentShader;
            string modelDirectory;
        };
    }
}

#endif /* MODELLOADER_H */
