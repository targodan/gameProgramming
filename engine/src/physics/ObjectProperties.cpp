#include "ObjectProperties.h"

namespace engine {
    namespace physics {
        ObjectProperties::ObjectProperties(const ObjectProperties& orig)
                : allVertices(orig.allVertices),
                surfaceVertexIndices(orig.surfaceVertexIndices),
                surfaceAreaPerVertex(orig.surfaceAreaPerVertex),
                massPerVertex(orig.massPerVertex) {}
        
        ObjectProperties::ObjectProperties(ObjectProperties&& orig)
                : allVertices(std::move(orig.allVertices)),
                surfaceVertexIndices(std::move(orig.surfaceVertexIndices)),
                surfaceAreaPerVertex(std::move(orig.surfaceAreaPerVertex)),
                massPerVertex(std::move(orig.massPerVertex)) {}
        
        ObjectProperties& ObjectProperties::operator=(const ObjectProperties& orig) {
            this->allVertices = orig.allVertices;
            this->surfaceVertexIndices = orig.surfaceVertexIndices;
            this->surfaceAreaPerVertex = orig.surfaceAreaPerVertex;
            this->massPerVertex = orig.massPerVertex;
            
            return *this;
        }
        ObjectProperties& ObjectProperties::operator=(ObjectProperties&& orig) {
            std::swap(this->allVertices, orig.allVertices);
            std::swap(this->surfaceVertexIndices, orig.surfaceVertexIndices);
            std::swap(this->surfaceAreaPerVertex, orig.surfaceAreaPerVertex);
            std::swap(this->massPerVertex, orig.massPerVertex);
            
            return *this;
        }
        
        VectorXf ObjectProperties::getSurfaceVertices() const {
            VectorXf surface(this->surfaceVertexIndices.size() * 3);
            size_t i = 0;
            for(auto index : this->surfaceVertexIndices) {
                surface(i++) = this->allVertices(index * 3 + 0);
                surface(i++) = this->allVertices(index * 3 + 1);
                surface(i++) = this->allVertices(index * 3 + 2);
            }
            return surface;
        }
                
        VectorXf ObjectProperties::mapSurfaceForcesToAllVertices(const VectorXf& surfaceForces) const {
            VectorXf forces = VectorXf::Zero(this->allVertices.rows());
            size_t i = 0;
            for(auto index : this->surfaceVertexIndices) {
                forces(index * 3 + 0) = surfaceForces(i + 0);
                forces(index * 3 + 1) = surfaceForces(i + 1);
                forces(index * 3 + 2) = surfaceForces(i + 2);
                i += 3;
            }
            return forces;
        }


        ObjectProperties ObjectProperties::DensityDistribution::uniformAreaDistribution(float totalArea) {
            auto numVertices = this->allVertices.rows() / 3;
            VectorXf areaPerVertex = MatrixXf::Ones(numVertices, 1) * (totalArea / numVertices);
            return ObjectProperties(this->allVertices, this->surfaceVertexIndices, this->massPerVertex, areaPerVertex);
        }
        ObjectProperties ObjectProperties::DensityDistribution::nonUniformAreaDistribution(const VectorXf& surfaceAreaPerVertex) {
            return ObjectProperties(this->allVertices, this->surfaceVertexIndices, this->massPerVertex, surfaceAreaPerVertex);
        }

        ObjectProperties::DensityDistribution ObjectProperties::UniformTetrahedronDistribution::uniformDensity(float totalVolume, float density) {
            auto numVertices = this->allVertices.rows() / 3;
            VectorXf massPerVertex = VectorXf::Ones(numVertices) * (totalVolume * density / numVertices);
            return DensityDistribution(this->allVertices, this->surfaceVertexIndices, massPerVertex);
        }
        ObjectProperties::DensityDistribution ObjectProperties::UniformTetrahedronDistribution::nonUniformDensity(const VectorXf& massPerVertex) {
            return DensityDistribution(this->allVertices, this->surfaceVertexIndices, massPerVertex);
        }

        ObjectProperties::UniformTetrahedronDistribution ObjectProperties::uniformTetrahedronDistribution(const VectorXf& allVertices, const vector<size_t>& surfaceVertexIndices) {
            return UniformTetrahedronDistribution(allVertices, surfaceVertexIndices);
        }
        
        VectorXf ObjectProperties::verticesToFlatVector(const engine::renderer::Mesh::ConstVertexProxy& vertices) {
            VectorXf vectors(vertices.size()*3, 1);
            size_t i = 0;
            for(auto& vertex : vertices) {
                vectors(i+0) = vertex.position.x;
                vectors(i+1) = vertex.position.y;
                vectors(i+2) = vertex.position.z;
                i += 3;
            }
            return vectors;
        }
        
        VectorXf ObjectProperties::verticesToFlatVector(const vector<engine::renderer::Vertex>& vertices) {
            VectorXf vectors(vertices.size()*3, 1);
            size_t i = 0;
            for(auto& vertex : vertices) {
                vectors(i+0) = vertex.position.x;
                vectors(i+1) = vertex.position.y;
                vectors(i+2) = vertex.position.z;
                i += 3;
            }
            return vectors;
        }
    }
}