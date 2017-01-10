#include "Mesh.h"
#include "VertexAttribute.h"

#include "../util/vec4.h"

namespace engine {
    namespace renderer {
        Mesh::Mesh(vector<Vertex> vertices, vector<GLuint> indices, DataUsagePattern usage) 
            : usage(usage), vao(std::make_unique<VertexArray>()), vertices(vertices), indices(indices), wasLoaded(false) {
            this->createVBO();
            this->createEBO();
        }
        
        Mesh::Mesh(const Mesh& orig)
            : usage(orig.usage), 
                vertices(orig.vertices), indices(orig.indices), wasLoaded(orig.wasLoaded) {
            this->material = orig.material==nullptr ? nullptr : std::make_shared<Material>(*(orig.material));
            this->vao = orig.vao==nullptr ? nullptr : std::make_unique<VertexArray>(*(orig.vao));
            
//            if(orig.wasLoaded) {
//                this->loadMesh();
//            }
//            
//            if(orig.material != nullptr) {
//                this->material
//            }
        }
        
        Mesh::Mesh(Mesh&& orig)
            : usage(std::move(orig.usage)), 
                vertices(std::move(orig.vertices)), indices(std::move(orig.indices)), wasLoaded(std::move(orig.wasLoaded)) {
            this->material = orig.material==nullptr ? nullptr : std::make_shared<Material>(*(orig.material));
            this->vao = std::make_unique<VertexArray>(*(orig.vao));
        }

        Mesh::~Mesh() {
            // this->releaseMesh();
        }
        
        Mesh& Mesh::operator=(const Mesh& right) {
            this->releaseMesh();
            
            this->usage = right.usage;
            this->vao = std::make_unique<VertexArray>(*(right.vao));
            this->material = std::make_shared<Material>(*(right.material));
            this->vertices = right.vertices;
            this->indices = right.indices;
            this->wasLoaded = false;
            return *this;
        }
        Mesh& Mesh::operator=(Mesh&& right) {
            std::swap(this->usage, right.usage);
            this->vao = std::make_unique<VertexArray>(*(right.vao));
            this->material = std::make_shared<Material>(*(right.material));
            std::swap(this->vertices, right.vertices);
            std::swap(this->indices, right.indices);
            std::swap(this->wasLoaded, right.wasLoaded);
            return *this;
        }
        
        void Mesh::render() {
#ifdef DEBUG
            if(this->material == nullptr) {
                throw WTFException("Cannot render mesh: no material set.");
            }
#endif /*DEBUG*/
            
            this->material->makeActive();
            
            this->vao->bind();
            //glDrawArrays(GL_TRIANGLES, 0, 3);
            this->vao->drawArrays();
            this->vao->unbind();
        }
        
        void Mesh::loadMesh() {
            this->vao->bind();
            this->vao->loadData();
            this->vao->unbind();
        }
        
        void Mesh::releaseMesh() {
            if(this->wasLoaded) {
                this->wasLoaded = false; 
            }
            vao->releaseVertexArray();
        }
        
        void Mesh::setMaterial(const std::shared_ptr<Material>& material) {
            this->material = material;
            
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
            attribs.push_back(positionAttrib);
            attribs.push_back(normalAttrib);
            attribs.push_back(textureCoordAttrib);
            
            // Look for first VBO to have no attributes set
#ifdef DEBUG
            bool foundEmptyVBO = false;
#endif /*DEBUG*/
            for(auto& vbo : this->vao->getVBOs()) {
                if(vbo->getAttributes().empty()) {
                    vbo->setAttributes(attribs);
                    
#ifdef DEBUG
                    foundEmptyVBO = true;
#endif /*DEBUG*/
                }
            }
#ifdef DEBUG
            if(!foundEmptyVBO) {
                throw WTFException("Could not set material: WTF did you do.");
            }
#endif /*DEBUG*/
            
            // Attach 
            this->vao->bind();
            this->vao->setAttributePointers();
            this->vao->unbind();
        }
        std::shared_ptr<const Material> Mesh::getMaterial() const {
            return this->material;
        }
        
        void Mesh::createVBO() {
            // Get no. of elements
            auto nVertices = this->vertices.size();
            
#ifdef DEBUG
            if(!nVertices) {
                throw WTFException("Cannot create VBO: no vertices given.");
            }
#endif /*DEBUG*/
            
            // Create VBO that holds vertex data
            auto vbo = std::make_unique<VertexBuffer>((void*) &this->vertices[0], sizeof(Vertex) * nVertices, nVertices, this->usage);
            this->vao->attachVBO(std::move(vbo));
        }
        void Mesh::createEBO(){
            // Get no. of elemnts
            auto nIndices = this->indices.size();
            
#ifdef DEBUG
            if(!nIndices) {
                throw WTFException("Cannot create EBO: no indices given.");
            }
#endif /*DEBUG*/
            
            // Create EBO that holds index data
            auto ebo = std::make_unique<ElementBuffer>((void*) &this->indices[0], sizeof(GLuint) * nIndices, nIndices, this->usage);
            this->vao->setEBO(std::move(ebo));
        }
        
        Mesh::VertexProxy Mesh::getVertices() {
            return VertexProxy(*this);
        }
        const Mesh::ConstVertexProxy Mesh::getVertices() const {
            return ConstVertexProxy(*this);
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


