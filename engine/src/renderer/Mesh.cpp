#include "Mesh.h"
#include "VertexAttribute.h"

#include "../util/vec4.h"

#include <easylogging++.h>

namespace engine {
    namespace renderer {
        Mesh::Mesh(vector<Vertex> vertices, DataUsagePattern usage) 
            : material(nullptr), vertices(vertices), vao(std::make_unique<VertexArray>()), loaded(false), usage(usage) {
            this->createVBO(this->vertices, usage);
        }
        Mesh::Mesh(vector<Vertex> vertices, vector<GLuint> indices, DataUsagePattern usage) 
            : material(nullptr), vertices(vertices), indices(indices), vao(std::make_unique<VertexArray>()), loaded(false), usage(usage) {
            this->createEBO(this->indices, usage);
            this->createVBO(this->vertices, usage);
        }
        
        Mesh::Mesh(const Mesh& orig)
            : material(orig.material), vertices(orig.vertices), indices(orig.indices), vao(std::make_unique<VertexArray>()), usage(orig.usage) {
            // this->material = orig.material==nullptr ? nullptr : std::make_shared<Material>(*(orig.material));
            // For testing: copy points to same material
            
            /*
             * The problem here is as follows.
             * Creating new buffers may not be intuitive in a copy, but if we don't do this
             * the pointer to the data will still be to orig.vertices, wich may result in
             * data corruption when calling loadData.
             * TODO: Fix this.
             */
            if(!this->indices.empty()) {
                this->createEBO(this->indices, this->usage);
            }
            this->createVBO(this->vertices, this->usage);
            
            if(orig.loaded) {
                this->loadMesh();
            }
        }
        
        Mesh::Mesh(Mesh&& orig)
            : material(std::move(orig.material)),
                vertices(std::move(orig.vertices)), indices(std::move(orig.indices)),
                vao(std::move(orig.vao)), loaded(std::move(orig.loaded)), usage(std::move(orig.usage)) {
        }
        
        Mesh& Mesh::operator=(const Mesh& right) {
            this->vao = std::make_unique<VertexArray>();
            this->material = right.material;
            this->vertices = right.vertices;
            this->indices = right.indices;
            this->usage = right.usage;
            
            if(!this->indices.empty()) {
                this->createEBO(this->indices, this->usage);
            }
            this->createVBO(this->vertices, this->usage);
            
            if(right.loaded) {
                this->loadMesh();
            }
            
            return *this;
        }
        Mesh& Mesh::operator=(Mesh&& right) {
            this->vao = std::make_unique<VertexArray>(*(right.vao));
            this->material = std::move(right.material);
            this->vertices = std::move(right.vertices);
            this->indices = std::move(right.indices);
            this->loaded = std::move(right.loaded);
            this->usage = std::move(right.usage);
            
            return *this;
        }

        Mesh::~Mesh() {
            
        }
        
        void Mesh::setVerticesChanged(bool changed) {
            this->verticesChanged = changed;
        }
        void Mesh::setIndicesChanged(bool changed) {
            this->indicesChanged = changed;
        }
        
        void Mesh::render() {
#ifdef DEBUG
            if(this->material == nullptr) {
                throw WTFException("Cannot render mesh: no material set.");
            }
#endif /*DEBUG*/
            
            this->vao->bind();
            
            if(this->verticesChanged) {
                this->vao->reloadData();
                this->verticesChanged = false;
            }
            if(this->indicesChanged) {
                if(this->indices.size() > 0) {
                    this->vao->reloadIndices();
                }
                this->indicesChanged = false;
            }
            
            this->vao->unbind();
            
            this->material->makeActive();
            
            this->vao->bind();
            
            if(this->indices.size() > 0) {
                this->vao->drawElements();
            } else {
                this->vao->drawArrays();
            }
            this->vao->unbind();
            
            this->material->makeInactive();
        }
        
        void Mesh::loadMesh() {
            this->vao->bind();
            this->vao->loadData();
            if(this->indices.size() > 0) {
                this->vao->loadIndices();
            }
            this->vao->unbind();
            
            this->loaded = true;
            this->verticesChanged = false;
            this->indicesChanged = false;
        }
        
        void Mesh::releaseMesh() {
            if(this->loaded) {
                this->loaded = false; 
            }
            vao->releaseVertexArray();
        }
        
        void Mesh::setMaterial(const std::shared_ptr<Material>& material) {
            this->material = material;
            
            this->setVAOAttributes();
            this->enableVAOAttributes();
        }
        std::shared_ptr<const Material> Mesh::getMaterial() const {
            return this->material;
        }
        bool Mesh::wasLoaded() const {
            return this->loaded;
        }
        
        void Mesh::createVBO(vector<Vertex>& vertices, DataUsagePattern usage) {
            // Get no. of elements
            auto nVertices = vertices.size();
            
#ifdef DEBUG
            if(!nVertices) {
                throw WTFException("Cannot create VBO: no vertices given.");
            }
#endif /*DEBUG*/
            
            // Create VBO that holds vertex data
            auto vbo = std::make_unique<VertexBuffer>(reinterpret_cast<void*>(vertices.data()), sizeof(Vertex) * nVertices, nVertices, usage);
            this->vao->attachVBO(std::move(vbo));
        }
        void Mesh::createEBO(vector<GLuint>& indices, DataUsagePattern usage){
            // Get no. of elements
            auto nIndices = this->indices.size();
            
#ifdef DEBUG
            if(!nIndices) {
                throw WTFException("Cannot create EBO: no indices given.");
            }
#endif /*DEBUG*/
            
            // Create EBO that holds index data
            auto ebo = std::make_unique<ElementBuffer>(reinterpret_cast<void*>(indices.data()), sizeof(GLuint) * nIndices, nIndices, usage);
            this->vao->setEBO(std::move(ebo));
        }
        void Mesh::setVAOAttributes() {
            // Retrieve attribute locations in shader program
            auto positionIndex = this->material->getShader()->getAttributeLocation("position");
            auto normalIndex = this->material->getShader()->getAttributeLocation("normal");
            auto textureCoordinateIndex = this->material->getShader()->getAttributeLocation("textureCoordinate");
            
            // State VBO attributes
            VertexAttribute positionAttrib {positionIndex, Vertex::nElements, 
                    DataType::FLOAT, 0, sizeof(Vertex), 
                    (GLvoid*)offsetof(Vertex, position)};

            VertexAttribute normalAttrib {normalIndex, Vertex::nElements, 
                    DataType::FLOAT, 0, sizeof(Vertex), 
                    (GLvoid*)offsetof(Vertex, normal)};

            VertexAttribute textureCoordAttrib {textureCoordinateIndex, Vertex::nElements, 
                    DataType::FLOAT, 0, sizeof(Vertex), 
                    (GLvoid*)offsetof(Vertex, textureCoord)};

            vector<VertexAttribute> attribs;
            if(positionIndex != -1) {
                attribs.push_back(positionAttrib);
            }
            if(normalIndex != -1) {
                attribs.push_back(normalAttrib);
            }
            if(textureCoordinateIndex != -1) {
                attribs.push_back(textureCoordAttrib);
            }
            
            // Look for first VBO to have no attributes set
#ifdef DEBUG
            bool foundEmptyVBO = false;
#endif /*DEBUG*/
            // for(auto& vbo : this->vao->getVBOs()) {
            auto& vbo = this->vao->getVBOs()[0];
            if(vbo->getAttributes().empty()) {
                vbo->setAttributes(attribs);
                    
#ifdef DEBUG
                    foundEmptyVBO = true;
#endif /*DEBUG*/
                }
            // }
#ifdef DEBUG
            if(!foundEmptyVBO) {
                throw WTFException("Could not set material: WTF did you do.");
            }
#endif /*DEBUG*/
        }
        void Mesh::enableVAOAttributes() {
            // Attach 
            this->vao->bind();
            this->vao->setAttributePointers();
            this->vao->unbind();
        }
        
        void Mesh::applyTransformation(glm::mat4 transformMatrix) {
            if(this->vertices.size() > 16) {
                this->applyTransformation_Parallel(transformMatrix);
            } else {
                this->applyTransformation_Sequential(transformMatrix);
            }
        }
        void Mesh::applyTransformation(glm::mat3 transformMatrix) {
            if(this->vertices.size() > 16) {
                this->applyTransformation_Parallel(transformMatrix);
            } else {
                this->applyTransformation_Sequential(transformMatrix);
            }
        }
        
        Mesh::VertexProxy Mesh::getVertices() {
            return VertexProxy(*this);
        }
        const Mesh::ConstVertexProxy Mesh::getVertices() const {
            return ConstVertexProxy(*this);
        }
        
        vector<GLuint>& Mesh::getFaceIndices() {
            return this->indices;
        }
        
        const vector<GLuint>& Mesh::getFaceIndices() const {
            return this->indices;
        }
        
        bool Mesh::isEdgePartOfFace(size_t faceIndex, GLuint vertIndexA, GLuint vertIndexB) const {
            auto indexOne = this->indices[faceIndex*3 + 0];
            auto indexTwo = this->indices[faceIndex*3 + 1];
            auto indexThree = this->indices[faceIndex*3 + 2];
            return         (indexOne == vertIndexA && indexTwo == vertIndexB)
                        || (indexOne == vertIndexA && indexThree == vertIndexB)
                        || (indexTwo == vertIndexA && indexThree == vertIndexB)
                        || (indexOne == vertIndexB && indexTwo == vertIndexA)
                        || (indexOne == vertIndexB && indexThree == vertIndexA)
                        || (indexTwo == vertIndexB && indexThree == vertIndexA);
        }
        
        void Mesh::deleteEdge(GLuint vertIndexA, GLuint vertIndexB) {
            bool deleteNext3 = false;
            this->indices.erase(std::remove_if(this->indices.begin(), this->indices.end(), [&,this](const auto& element) {
                // This fells so dirty.
                auto index = &element - &*this->indices.begin();
                if(index % 3 == 0) {
                    // start of face => check it
                    deleteNext3 = this->isEdgePartOfFace(index / 3, vertIndexA, vertIndexB);
                }
                return deleteNext3;
            }), this->indices.end());
            this->vao->getEBO().setNumberOfElements(this->indices.size());
            this->setIndicesChanged(true);
        }
        
        void Mesh::deleteEdges(const vector<std::pair<GLuint, GLuint>>& edges) {
//            bool deleteNext3 = false;
//            this->indices.erase(std::remove_if(this->indices.begin(), this->indices.end(), [&,this](const auto& element) {
//                // This fells so dirty.
//                auto index = &element - &*this->indices.begin();
//                if(index % 3 == 0) {
//                    // start of face => check it
//                    deleteNext3 = false;
//                    for(auto& edge : edges) {
//                        if(this->isEdgePartOfFace(index / 3, edge.first, edge.second)) {
//                            deleteNext3 = true;
//                            break;
//                        }
//                    }
//                }
//                return deleteNext3;
//            }), this->indices.end());
//            this->vao->getEBO().setNumberOfElements(this->indices.size());
//            this->setIndicesChanged(true);
            
            size_t size = this->indices.size();
            for(GLuint* elem = &this->indices[0]; elem <= &this->indices[size-1]; elem += 3) {
                const GLuint* elem0 = elem+0;
                const GLuint* elem1 = elem+1;
                const GLuint* elem2 = elem+2;
                for(auto& edge : edges) {
                    if(    (*(elem0) == edge.first  && *(elem1) == edge.second)
                        || (*(elem0) == edge.first  && *(elem2) == edge.second)
                        || (*(elem1) == edge.first  && *(elem2) == edge.second)
                        || (*(elem0) == edge.second && *(elem1) == edge.first)
                        || (*(elem0) == edge.second && *(elem2) == edge.first)
                        || (*(elem1) == edge.second && *(elem2) == edge.first)) {
                        // Move to end
                        for(GLuint* it = elem; it <= &this->indices[size-1-3]; it += 3) {
                            *(it+0) = *(it+3);
                            *(it+1) = *(it+4);
                            *(it+2) = *(it+5);
                        }
                        size -= 3;
                        break;
                    }
                }
            }
            this->indices.erase(this->indices.begin()+size, this->indices.end());
        }
    }
}


