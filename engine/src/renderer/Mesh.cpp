#include "Mesh.h"

#include "../util/vec4.h"

namespace engine {
    namespace renderer {
        Mesh::Mesh(Array<Vertex> vertices, Array<GLuint> indices, GLenum usage) 
            : vertices(vertices), indices(indices), wasLoaded(false) {}
        
        Mesh::Mesh(const Mesh& orig)
            : usage(orig.usage), vertices(orig.vertices), indices(orig.indices), wasLoaded(false) {}
        
        Mesh::Mesh(Mesh&& orig)
            : usage(std::move(orig.usage)), vao(std::move(orig.vao)), vbo(std::move(orig.vbo)),
                vertices(std::move(orig.vertices)), indices(std::move(orig.indices)), wasLoaded(std::move(orig.wasLoaded)) {}

        Mesh::~Mesh() {
            this->releaseMesh();
        }
        
        Mesh& Mesh::operator=(const Mesh& right) {
            this->releaseMesh();
            
            this->usage = right.usage;
            this->vao = right.vao;
            this->vbo = right.vbo;
            this->vertices = right.vertices;
            this->indices = right.indices;
            this->wasLoaded = false;
            return *this;
        }
        
        Mesh& Mesh::operator=(Mesh&& right) {
            std::swap(this->usage, right.usage);
            std::swap(this->vao, right.vao);
            std::swap(this->vbo, right.vbo);
            std::swap(this->vertices, right.vertices);
            std::swap(this->indices, right.indices);
            std::swap(this->wasLoaded, right.wasLoaded);
            return *this;
        }
        
        void Mesh::initMesh() {
            // Generate and bind VBO & VAO
            glGenVertexArrays(1, &this->vao);
            glGenBuffers(1, &this->vbo);
            
            glBindVertexArray(this->vao);
            glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
            
            // Store data in bound buffer
            glBufferData(GL_ARRAY_BUFFER, (GLint) sizeof(Vertex)*(this->vertices.size()), &this->vertices, this->usage);
            
            // Specify vertex attributes for VAO in bound vertex array
            glEnableVertexAttribArray(Vertex::posLoc);
            glVertexAttribPointer(Vertex::posLoc, 3, GL_FLOAT, GL_FALSE, (GLint) sizeof(Vertex), (GLvoid*) offsetof(Vertex, position));
            
            glEnableVertexAttribArray(Vertex::normLoc);
            glVertexAttribPointer(Vertex::normLoc, 3, GL_FLOAT, GL_FALSE, (GLint) sizeof(Vertex), (GLvoid*) offsetof(Vertex, normal));
            
            glEnableVertexAttribArray(Vertex::texLoc);
            glVertexAttribPointer(Vertex::texLoc, 3, GL_FLOAT, GL_FALSE, (GLint) sizeof(Vertex), (GLvoid*) offsetof(Vertex, textureCoord));
            
            // Unbind buffers
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
        
        void Mesh::releaseMesh() {
            if(this->wasLoaded) {
                glDeleteBuffers(1, &this->vbo);
                glDeleteVertexArrays(1, &this->vao);
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


