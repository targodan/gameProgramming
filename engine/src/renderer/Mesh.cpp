#include "Mesh.h"
#include "VertexAttribute.h"

#include "../util/vec4.h"

namespace engine {
    namespace renderer {
        Mesh::Mesh(vector<Vertex> vertices, vector<GLuint> indices, DataUsagePattern usage) 
            : usage(usage), vertices(vertices), indices(indices), wasLoaded(false) {
            
            // Get no. of elements
            auto nVertices = this->vertices.size();
            auto nIndices = this->indices.size();
            
            // Create VBO that holds vertex data
            VertexBuffer vbo {(void*) &this->vertices[0], sizeof(Vertex) * nVertices, nVertices, usage};
            auto vboPtr = std::make_unique<VertexBuffer>(vbo);
            
            // State VBO attributes
            VertexAttribute positionAttrib {Vertex::posLoc, Vertex::nElements, 
                    DataType::FLOAT, 0, sizeof(Vertex), 
                    (GLvoid*)offsetof(Vertex, position)};

            VertexAttribute normalAttrib {Vertex::normLoc, Vertex::nElements, 
                    DataType::FLOAT, 0, sizeof(Vertex), 
                    (GLvoid*)offsetof(Vertex, normal)};

            VertexAttribute textureCoordAttrib {Vertex::texLoc, Vertex::nElements, 
                    DataType::FLOAT, 0, sizeof(Vertex), 
                    (GLvoid*)offsetof(Vertex, textureCoord)};

            vector<VertexAttribute> attribs;
            attribs.push_back(positionAttrib);
            attribs.push_back(normalAttrib);
            attribs.push_back(textureCoordAttrib);
            vbo.setAttributes(attribs);
            
            // Attach VBO to VAO
            this->vao.bind();
            
            this->vao.attachVBO(std::move(vboPtr));
            this->vao.setAttributePointers();
            
            this->vao.unbind();
        }
        
        Mesh::Mesh(const Mesh& orig)
            : usage(orig.usage), vao(orig.vao), 
                vertices(orig.vertices), indices(orig.indices), wasLoaded(false) {
            // NOTE: The copy gets loaded iff the original was loaded.
            if(orig.wasLoaded) {
                this->loadMesh();
            }
        }
        
        Mesh::Mesh(Mesh&& orig)
            : usage(std::move(orig.usage)), vao(std::move(orig.vao)), 
                vertices(std::move(orig.vertices)), indices(std::move(orig.indices)), wasLoaded(std::move(orig.wasLoaded)) {
        
        }

        Mesh::~Mesh() {
            this->releaseMesh();
        }
        
        Mesh& Mesh::operator=(const Mesh& right) {
            this->releaseMesh();
            
            this->usage = right.usage;
            this->vao = right.vao;
            this->vertices = right.vertices;
            this->indices = right.indices;
            this->wasLoaded = false;
            return *this;
        }
        
        Mesh& Mesh::operator=(Mesh&& right) {
            std::swap(this->usage, right.usage);
            std::swap(this->vao, right.vao);
            std::swap(this->vertices, right.vertices);
            std::swap(this->indices, right.indices);
            std::swap(this->wasLoaded, right.wasLoaded);
            return *this;
        }
        
        void Mesh::loadMesh() {
            this->vao.bind();
            this->vao.loadData();
            this->vao.unbind();
        }
        
        void Mesh::releaseMesh() {
            if(this->wasLoaded) {
                this->wasLoaded = false; // TODO?
            }
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
    }
}


