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
            
            Mesh() : loaded(false) {}
            Mesh(vector<Vertex> vertices, DataUsagePattern usage = DataUsagePattern::STATIC_DRAW);
            Mesh(vector<Vertex> vertices, vector<GLuint> indices, DataUsagePattern usage = DataUsagePattern::STATIC_DRAW);
            
            Mesh(const Mesh& orig);
            Mesh(Mesh&& orig);

            Mesh& operator=(const Mesh& right);
            Mesh& operator=(Mesh&& right);
            
            virtual ~Mesh();
            
            virtual Mesh* clone() {
                return new Mesh(*this);
            };
            
            virtual void loadMesh();
            virtual void render();
            
            void setVerticesChanged(bool changed);
            void setIndicesChanged(bool changed);
            void releaseMesh();
            
            bool wasLoaded() const;
            
            virtual void setMaterial(const std::shared_ptr<Material>& material);
            virtual void updateBuffer();
            std::shared_ptr<const Material> getMaterial() const;
            std::shared_ptr<Material> getMaterial();
            
            void applyTransformation(glm::mat3 transformMatrix);
            void applyTransformation(glm::mat4 transformMatrix);
            
            VertexProxy getVertices();
            const ConstVertexProxy getVertices() const;

            vector<GLuint>& getFaceIndices();
            const vector<GLuint>& getFaceIndices() const;
            
            void deleteEdge(GLuint vertIndexA, GLuint vertIndexB);
            void deleteEdges(const vector<std::pair<GLuint, GLuint>>& edges);
        protected:
            void createVBO(vector<Vertex>& vertices, DataUsagePattern usage);
            void createEBO(vector<GLuint>& indices, DataUsagePattern usage);
            void enableVAOAttributes();
            virtual void setVAOAttributes();
            
            bool isEdgePartOfFace(size_t faceIndex, GLuint vertIndexA, GLuint vertIndexB) const;
            
            std::shared_ptr<Material> material;
            
            vector<Vertex> vertices;
            vector<GLuint> indices;
            
            std::unique_ptr<VertexArray> vao;
            
            bool loaded;
            bool verticesChanged = false;
            bool indicesChanged = false;
            
            DataUsagePattern usage;
            
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

