#include "Mesh.h"

namespace engine {
    namespace renderer {
        Mesh::Mesh(Array<Vertex> vertices, Array<GLuint> indices, GLenum usage) 
            : vertices(vertices), indices(indices)
        {
            this->nVertices = vertices.size();
            this->initMesh();
        }   

        Mesh::~Mesh() {
            this->releaseMesh();
        }
        
        void Mesh::initMesh() {
            // Generate and bind VBO & VAO
            glGenVertexArrays(1, &this->vao);
            glGenBuffers(1, &this->vbo);
            
            glBindVertexArray(this->vao);
            glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
            
            // Store data in bound buffer
            glBufferData(GL_ARRAY_BUFFER, (GLint) sizeof(Vertex)*(this->nVertices), &this->vertices, this->usage);
            
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
            glDeleteBuffers(1, &this->vbo);
            glDeleteVertexArrays(1, &this->vao);
        }
    }
}


