#include "ModelLoader.h"
#include "ImageFormat.h"
#include "DefaultShader.h"
#include <string>

namespace engine {
    namespace renderer {
        ModelLoader::ModelLoader(string pathToModel, Mapping mapping, bool lighting) 
            : mapping(mapping), lighting(lighting) {
            if(mapping != Mapping::NONE && mapping != Mapping::NORMAL_MAPPING) {
                throw WTFException("ModelLoader error: only normal mapping or not mapping at all are supported now.");
            }
            
            this->modelDirectory = pathToModel.substr(0, pathToModel.find_last_of("/")) + "/";
            
            // TODO: Test flags: aiProcess_OptimizeGraph, aiProcess_OptimizeMeshes
            Importer modelImporter;
            const aiScene* scene = modelImporter.ReadFile(pathToModel, aiProcess_Triangulate);

            if(!scene) {
                std::ostringstream error; 
                error << "Could not import model: " << modelImporter.GetErrorString();
                throw IOException(error.str().c_str());
            }

            this->processNode(scene->mRootNode, scene);
        }

        void ModelLoader::processNode(aiNode* node, const aiScene* scene) {
            if(scene->mNumMeshes > 1) {
                throw WTFException("Could not load model: model consists of multiple meshes which is not supported yet.");
            }
            
            auto assimpMesh = this->findAssimpMesh(node, scene);
            this->mesh = this->createMeshObject(assimpMesh, scene);
            this->material = this->createMaterialObject(assimpMesh, scene);
        }
        
        aiMesh* ModelLoader::findAssimpMesh(aiNode* node, const aiScene* scene) {
            if(node->mNumMeshes > 0) {
                return scene->mMeshes[node->mMeshes[0]];
            } else {
                for(unsigned int i = 0; i < node->mNumChildren; i++) {
                    auto mesh = this->findAssimpMesh(node->mChildren[i], scene); // Recursively run through all children until one does not return a nullptr
                    if(mesh != nullptr) {
                        return mesh;
                    }
                }
                return nullptr;
            }
        }

        std::shared_ptr<Mesh> ModelLoader::createMeshObject(aiMesh* mesh, const aiScene* scene) {
            vector<Vertex> vertices;
            vector<GLuint> indices;

            // Get vertex data
            for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
                Vertex vertex;
                vertex.position = util::vec3{mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
                vertex.normal = util::vec3{mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};
                if(mesh->mTextureCoords[0]) {
                    vertex.textureCoord = util::vec2{mesh->mTextureCoords[0][i].x, 1-mesh->mTextureCoords[0][i].y};
                }
                vertices.push_back(vertex);
            }

            // Get indices from faces
            for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
                for(unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++) {
                    indices.push_back(mesh->mFaces[i].mIndices[j]);
                }
            }

            std::shared_ptr<Mesh> meshPtr = std::make_shared<Mesh>(vertices, indices);
            return meshPtr;
        }

        std::shared_ptr<Material> ModelLoader::createMaterialObject(aiMesh* mesh, const aiScene* scene) {
            if(mesh->mMaterialIndex < 0) {
                throw WTFException("Currently processed mesh has no associated material.");
            } 

            aiMaterial* assimpMaterial = scene->mMaterials[mesh->mMaterialIndex];
            
            
            vector<Texture> textures;
            vector<aiTextureType> textureTypes;
            
            textureTypes.push_back(aiTextureType_DIFFUSE);
            if(this->lighting) {
                textureTypes.push_back(aiTextureType_SPECULAR);
                if(this->mapping == Mapping::NORMAL_MAPPING) {
                    textureTypes.push_back(aiTextureType_NORMALS);
                }
            }
            
            vector<Texture> result;
            for(auto textureType : textureTypes) {
                try {
                    result = this->createTexturesOfType(assimpMaterial, textureType);
                    textures.insert(textures.end(), result.begin(), result.end());
                } catch(...) {
                    std::string errorType;
                    switch(textureType) {
                        case aiTextureType_DIFFUSE:
                            errorType = "DIFFUSE"; break;
                        case aiTextureType_SPECULAR:
                            errorType = "SPECULAR"; break;
                        case aiTextureType_NORMALS:
                            errorType = "NORMAL"; break;
                        default:
                            break; // Dafuq?
                    }
                    
                    LOG(WARNING) << "ModelLoader: Could not import texture of type " << errorType;
                }
            }
            
//            Texture texture = {"resources/textures/bomb_diffuse.png", ImageFormat::RGBA};
//            vector<Texture> textures;
//            textures.push_back(texture);
            
//            std::string vertexShader = "";
//            std::string fragmentShader = "";
//            std::shared_ptr<Material> material;
//            if(pathToVertexShader == "" && pathToFragmentShader == "") {
//                if(!textures.empty()) {
//                    vertexShader = DefaultShader::createSimpleTextureVertexShader();
//                    fragmentShader = DefaultShader::createSimpleTextureFragmentShader();
//                } else {
//                    vertexShader = DefaultShader::createFlatVertexShader();
//                    fragmentShader = DefaultShader::createFlatFragmentShader();
//                }
//                
//                material = std::make_shared<Material>(std::make_shared<ShaderProgram>(ShaderProgram::createShaderProgramFromSource(vertexShader, fragmentShader)));
//            } else {
//                material = std::make_shared<Material>(std::make_shared<ShaderProgram>(pathToVertexShader, pathToFragmentShader));
//            }
            
            std::shared_ptr<Material> material;
            if(!this->lighting) {
                std::string vertexShader = "";
                std::string fragmentShader = "";
                
                if(!textures.empty()) {
                    vertexShader = DefaultShader::createSimpleTextureVertexShader();
                    fragmentShader = DefaultShader::createSimpleTextureFragmentShader();
                } else {
                    vertexShader = DefaultShader::createFlatVertexShader();
                    fragmentShader = DefaultShader::createFlatFragmentShader();
                }
                
                material = std::make_shared<Material>(std::make_shared<ShaderProgram>(ShaderProgram::createShaderProgramFromSource(vertexShader, fragmentShader)));
            } else {
                material = std::make_shared<Material>();
            }
                
            if(!textures.empty()) {
                material->setTextures(textures);
            }

            return material;
        }

        vector<Texture> ModelLoader::createTexturesOfType(aiMaterial* assimpMaterial, aiTextureType type) {
            vector<Texture> textures;
//            for(unsigned int i = 0; i < assimpMaterial->GetTextureCount(type); i++) {
//                aiString pathToTexture;
//                assimpMaterial->GetTexture(type, i, &pathToTexture); // TODO: Retrieve other parameters: mapping, uvindex, blend, op, mapmode
//                
//                Texture texture = {this->modelDirectory + std::string(pathToTexture.C_Str()), ImageFormat::RGBA, ImageFormat::RGBA};
//                textures.push_back(texture);
//            }
            if(assimpMaterial->GetTextureCount(type) > 1) {
                LOG(WARNING) << "ModelLoader: Multiple textures for a unique texture type detected. Will only use the first one found.";
            }
            
            TextureType myType;
            switch(type) {
                case aiTextureType_DIFFUSE:
                    myType = TextureType::DIFFUSE; break;
                case aiTextureType_SPECULAR:
                    myType = TextureType::SPECULAR; break;
                case aiTextureType_NORMALS:
                    myType = TextureType::NORMAL; break;
                default:
                    throw WTFException("Could not import model: Can only import diffuse, specular or normal textures.");
            }
            
            aiString pathToTexture;
            assimpMaterial->GetTexture(type, 0, &pathToTexture); // TODO: Retrieve other parameters: mapping, uvindex, blend, op, mapmode
                
            Texture texture = {this->modelDirectory + std::string(pathToTexture.C_Str()), ImageFormat::RGBA, ImageFormat::RGBA, TextureDimension::TEXTURE_2D, myType};
            textures.push_back(texture);

            return textures;
        }
    }
}
