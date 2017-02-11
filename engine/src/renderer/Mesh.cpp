#include "Mesh.h"
#include "VertexAttribute.h"

#include "../util/vec4.h"

#include <easylogging++.h>

namespace engine {
    namespace renderer {
        Mesh::Mesh(vector<Vertex> vertices, DataUsagePattern usage) 
            : material(nullptr), vertices(vertices), vao(std::make_unique<VertexArray>()), usage(usage) {
            this->createVBO(this->vertices, usage);
        }
        Mesh::Mesh(vector<Vertex> vertices, vector<GLuint> indices, DataUsagePattern usage) 
            : material(nullptr), vertices(vertices), indices(indices), vao(std::make_unique<VertexArray>()), usage(usage) {
            this->calculateTangentBasis();
            this->createEBO(this->indices, usage);
            this->createVBO(this->vertices, usage);
        }
        
        Mesh::Mesh(const Mesh& orig)
            : material(orig.material), vertices(orig.vertices), indices(orig.indices), tangents(orig.tangents), 
              vao(std::make_unique<VertexArray>()), usage(orig.usage) {
            
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
            : material(std::move(orig.material)), vertices(std::move(orig.vertices)), indices(std::move(orig.indices)),
              tangents(std::move(orig.tangents)), vao(std::move(orig.vao)), loaded(std::move(orig.loaded)), 
              verticesChanged(std::move(orig.verticesChanged)), indicesChanged(std::move(orig.indicesChanged)), usage(std::move(orig.usage)) {
            
        }
        
        Mesh& Mesh::operator=(const Mesh& right) {
            this->material = right.material;
            this->vertices = right.vertices;
            this->indices = right.indices;
            this->tangents = right.tangents;
            this->vao = std::make_unique<VertexArray>();
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
            this->material = std::move(right.material);
            this->vertices = std::move(right.vertices);
            this->indices = std::move(right.indices);
            this->tangents = std::move(right.tangents);
            this->vao = std::make_unique<VertexArray>(*(right.vao));
            this->loaded = std::move(right.loaded);
            this->verticesChanged = std::move(right.verticesChanged);
            this->indicesChanged = std::move(right.indicesChanged);
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
            
//            if(material->isNewShaderAttached()) {
                this->updateBuffer();
//            }
            
            if(!material->areTexturesLoaded() && !material->getTextures().empty()) {
                material->loadTextures();
            }
            
            this->material->makeActive();
            
            this->vao->bind();
            
            if(this->indices.size() > 0) {
                this->vao->drawElements();
            } else {
                this->vao->drawArrays();
            }
            this->vao->unbind();
            
            this->material->makeInactive();
            
            this->material->setNewShaderAttached(false);
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
        
        void Mesh::updateBuffer() {
            if(!this->material) {
                throw WTFException("Could not update buffer: not material set.");
            }
            
            this->setVAOAttributes();
            this->enableVAOAttributes();
        }
        void Mesh::setMaterial(const std::shared_ptr<Material>& material) {
            this->material = material;
            
            this->updateBuffer();
        }
        std::shared_ptr<const Material> Mesh::getMaterial() const {
            return this->material;
        }
        std::shared_ptr<Material> Mesh::getMaterial() {
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
        void Mesh::createTBO(vector<Mesh::Tangents>& tangents, DataUsagePattern usage) {
            // Create tangent VBO
            if(tangents.size() > 0) {
                this->vao->attachVBO(std::make_unique<VertexBuffer>(
                        reinterpret_cast<void*>(tangents.data()),
                        sizeof(Tangents) * tangents.size(),
                        tangents.size(),
                        usage));
            }
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
            auto tangentIndex = this->material->getShader()->getAttributeLocation("tangent");
            auto bitangentIndex = this->material->getShader()->getAttributeLocation("bitangent");
            
            // State VBO attributes
            VertexAttribute positionAttrib {positionIndex, Vertex::nElements, 
                    DataType::FLOAT, 0, sizeof(Vertex), 
                    (GLvoid*)offsetof(Vertex, position)};

            VertexAttribute normalAttrib {normalIndex, Vertex::nElements, 
                    DataType::FLOAT, 1, sizeof(Vertex), 
                    (GLvoid*)offsetof(Vertex, normal)};

            VertexAttribute textureCoordAttrib {textureCoordinateIndex, Vertex::nElements, 
                    DataType::FLOAT, 0, sizeof(Vertex), 
                    (GLvoid*)offsetof(Vertex, textureCoord)};
                
            vector<VertexAttribute> attribs;
            if(this->tangents.size() > 0 && tangentIndex != -1 && bitangentIndex != -1) {
                attribs.push_back({
                        tangentIndex,
                        3,
                        DataType::FLOAT,
                        GL_FALSE,
                        sizeof(Tangents),
                        (GLvoid*)offsetof(Tangents, tangent)
                    });
                attribs.push_back({
                        bitangentIndex,
                        3,
                        DataType::FLOAT,
                        GL_FALSE,
                        sizeof(Tangents),
                        (GLvoid*)offsetof(Tangents, bitangent)
                    });
            }    
            if(positionIndex != -1) {
                attribs.push_back(positionAttrib);
            }
            if(normalIndex != -1) {
                attribs.push_back(normalAttrib);
            }
            if(textureCoordinateIndex != -1) {
                attribs.push_back(textureCoordAttrib);
            }
            
            auto& vbo = this->vao->getVBOs()[0];
//            if(vbo->getAttributes().empty()) {
//                vbo->setAttributes(attribs);
//            }
            vbo->setAttributes(attribs);
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
            vector<GLuint> newIndices;
            newIndices.reserve(this->indices.size());
            
            const GLuint* end = &this->indices[this->indices.size()-1];
            for(GLuint* elem = &this->indices[0]; elem <= end; elem += 3) {
                const GLuint* elem0 = elem+0;
                const GLuint* elem1 = elem+1;
                const GLuint* elem2 = elem+2;
                bool deleteThis = false;
                for(auto& edge : edges) {
                    if(    (*(elem0) == edge.first  && *(elem1) == edge.second)
                        || (*(elem0) == edge.first  && *(elem2) == edge.second)
                        || (*(elem1) == edge.first  && *(elem2) == edge.second)
                        || (*(elem0) == edge.second && *(elem1) == edge.first)
                        || (*(elem0) == edge.second && *(elem2) == edge.first)
                        || (*(elem1) == edge.second && *(elem2) == edge.first)) {
                        deleteThis = true;
                        break;
                    }
                }
                if(!deleteThis) {
                    newIndices.push_back(*elem0);
                    newIndices.push_back(*elem1);
                    newIndices.push_back(*elem2);
                }
            }
            this->indices = std::move(newIndices);
        }
        
        void Mesh::calculateTangentBasis() {
            // Code from http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping/
            this->tangents.clear();
            this->tangents.resize(this->vertices.size(), {{0,0,0}, {0,0,0}});
            vector<size_t> counts(this->vertices.size(), 0);
            
            for(size_t faceIndex = 0; faceIndex < this->indices.size(); faceIndex += 3) {
                auto i0 = this->indices[faceIndex + 0];
                auto i1 = this->indices[faceIndex + 1];
                auto i2 = this->indices[faceIndex + 2];
                
                auto v0 = this->vertices[i0];
                auto v1 = this->vertices[i1];
                auto v2 = this->vertices[i2];
                
                auto p01 = v1.position - v0.position;
                auto p02 = v2.position - v0.position;
                
                auto uv01 = v1.textureCoord - v0.textureCoord;
                auto uv02 = v2.textureCoord - v0.textureCoord;
                
                float r = 1. / (uv01.x * uv02.y - uv01.y * uv02.x);
                auto tangent = r * (p01 * uv02.y - p02 * uv01.y);
                auto bitangent = r * (p02 * uv01.x - p01 * uv02.x);
                
                this->tangents[i0].tangent += tangent;
                this->tangents[i0].bitangent += bitangent;
                ++(counts[i0]);
                this->tangents[i1].tangent += tangent;
                this->tangents[i1].bitangent += bitangent;
                ++(counts[i1]);
                this->tangents[i2].tangent += tangent;
                this->tangents[i2].bitangent += bitangent;
                ++(counts[i2]);
            }
            
            // And average
            for(size_t i = 0; i < this->tangents.size(); ++i) {
                this->tangents[i].tangent /= static_cast<float>(counts[i]);
                this->tangents[i].bitangent /= static_cast<float>(counts[i]);
            }
        }
    }
}


