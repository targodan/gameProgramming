#ifndef MESH_H
#define MESH_H

#include "../util/Array.h"
#include "Vertex.h"
#include "gl/gl_core_3_3.h"
#include <glm/matrix.hpp>

namespace engine {
    namespace renderer {
        using namespace util;
        using namespace gl;
        
        /**
         * Mesh holds a mesh and handles loading it into the graphics card.
         * NOTE:    The mesh does NOT get loaded into the graphics card automatically,
         *          but it DOES get released automatically.
         *          Following this, the copy of an already loaded mesh does NOT
         *          get loaded automatically either.
         */
        class Mesh {
        public:
            Mesh(Array<Vertex> vertices, Array<GLuint> indices, GLenum usage = GL_STATIC_DRAW);
            
            Mesh(const Mesh& orig);
            Mesh(Mesh&& orig);

            Mesh& operator=(const Mesh& right);
            Mesh& operator=(Mesh&& right);
            
            virtual ~Mesh();
            
            std::string getUsage() const;
            
            void loadMesh();
            void releaseMesh();
            
            void applyTransformation(glm::mat3 transformMatrix);
            void applyTransformation(glm::mat4 transformMatrix);
            
        protected:
            GLenum usage;
            
            GLuint vao, vbo;
            
            Array<Vertex> vertices;
            Array<GLuint> indices;
            
            bool wasLoaded;
            
            template<class mat_t>
            void applyTransformation_Parallel(mat_t transformMatrix) {
                #pragma omp parallel for schedule(static)
                for(size_t i = 0; i < this->vertices.size(); ++i) {
                    this->vertices[i].position = transformMatrix * this->vertices[i].position;
                    this->vertices[i].normal = transformMatrix * this->vertices[i].normal;
                }
            }
            template<class mat_t>
            void applyTransformation_Sequential(mat_t transformMatrix) {
                for(size_t i = 0; i < this->vertices.size(); ++i) {
                    this->vertices[i].position = transformMatrix * this->vertices[i].position;
                    this->vertices[i].normal = transformMatrix * this->vertices[i].normal;
                }
            }
        };
    }
}


#endif /* MESH_H */

