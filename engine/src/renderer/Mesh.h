#ifndef MESH_H
#define MESH_H

#include "Material.h"
#include "util/Array.h"
#include "util/vector.h"
#include "Vertex.h"
#include "VertexArray.h"
#include "DataUsagePattern.h"
#include "gl/gl_core_3_3.h"
#include <glm/matrix.hpp>
#include <vector>

namespace engine {
    namespace renderer {
        using util::Array;
        using util::vector;
        using std::unique_ptr;
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
            Mesh() : wasLoaded(false) {}
            Mesh(vector<Vertex> vertices, vector<GLuint> indices, 
                    DataUsagePattern usage = DataUsagePattern::STATIC_DRAW);
            
            Mesh(const Mesh& orig);
            Mesh(Mesh&& orig);

            Mesh& operator=(const Mesh& right);
            Mesh& operator=(Mesh&& right);
            
            virtual ~Mesh();
            
            void render(const Material& material);
            
            std::string getUsage() const;
            
            void loadMesh();
            void releaseMesh();
            
            void applyTransformation(glm::mat3 transformMatrix);
            void applyTransformation(glm::mat4 transformMatrix);
            
        protected:
            DataUsagePattern usage;
            
            VertexArray vao;
            
            vector<Vertex> vertices;
            vector<GLuint> indices;
            
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

