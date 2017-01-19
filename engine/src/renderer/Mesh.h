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
            class ConstVertexProxy {
                const Mesh& m;
                ConstVertexProxy(const Mesh& m) : m(m) {}
                ConstVertexProxy(const Mesh&& m) : m(m) {}
                
                friend class Mesh;
            public:
                const Vertex& operator[](size_t i) const {
                    return m.vertices[i];
                }
                
                size_t size() const {
                    return m.vertices.size();
                }
                
                vector<Vertex>::const_iterator begin() const {
                    return m.vertices.begin();
                }
                
                vector<Vertex>::const_iterator end() const {
                    return m.vertices.end();
                }
            };
            class VertexProxy {
                Mesh& m;
                VertexProxy(Mesh& m) : m(m) {}
                VertexProxy(Mesh&& m) : m(m) {}
                
                friend class Mesh;
            public:
                Vertex& operator[](size_t i) {
                    return m.vertices[i];
                }
                
                size_t size() const {
                    return m.vertices.size();
                }
                
                vector<Vertex>::iterator begin() {
                    return m.vertices.begin();
                }
                
                vector<Vertex>::iterator end() {
                    return m.vertices.end();
                }
                
                operator ConstVertexProxy() const {
                    return ConstVertexProxy(this->m);
                }
            };
            
            Mesh() : wasLoaded(false) {}
            Mesh(vector<Vertex> vertices, 
                    DataUsagePattern usage = DataUsagePattern::STATIC_DRAW);
            Mesh(vector<Vertex> vertices, vector<GLuint> indices, 
                    DataUsagePattern usage = DataUsagePattern::STATIC_DRAW);
            
            Mesh(const Mesh& orig);
            Mesh(Mesh&& orig);

            Mesh& operator=(const Mesh& right);
            Mesh& operator=(Mesh&& right);
            
            virtual ~Mesh();
            
            void render();
            
            void loadMesh();
            void releaseMesh();
            
            std::string getUsage() const;
            
            void setMaterial(const std::shared_ptr<Material>& material);
            std::shared_ptr<const Material> getMaterial() const;
            
            void applyTransformation(glm::mat3 transformMatrix);
            void applyTransformation(glm::mat4 transformMatrix);
            
            VertexProxy getVertices();
            const ConstVertexProxy getVertices() const;
        private:
            void createVBO();
            void createEBO();
            
            std::shared_ptr<Material> material;
            
            DataUsagePattern usage;
            
            std::unique_ptr<VertexArray> vao;
            
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

