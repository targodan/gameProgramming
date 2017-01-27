#ifndef OBJECTPROPERTIES_H
#define OBJECTPROPERTIES_H

#include <eigen3/Eigen/Eigen>

#include "../util/vector.h"
#include "../renderer/Vertex.h"
#include "../renderer/Mesh.h"
#include "../IllegalArgumentException.h"

namespace engine {
    namespace physics {
        using namespace Eigen;
        using engine::util::vector;
        
        struct ObjectProperties {
            ObjectProperties(const VectorXf& allVertices,
                    const vector<size_t>& surfaceVertexIndices,
                    const VectorXf& surfaceAreaPerVertex,
                    const VectorXf& massPerVertex)
                    : allVertices(allVertices),
                    surfaceVertexIndices(surfaceVertexIndices),
                    surfaceAreaPerVertex(surfaceAreaPerVertex),
                    massPerVertex(massPerVertex) {
                if(surfaceVertexIndices.size() > static_cast<size_t>(allVertices.rows()) / 3) {
                    throw IllegalArgumentException("You specified more vertices to be part of the surface than there are vertices.");
                }
                if(static_cast<size_t>(surfaceAreaPerVertex.rows()) != surfaceVertexIndices.size()) {
                    throw IllegalArgumentException("There should be equal amounts of surface vertices and elements in surfaceAreaPerVertex.");
                }
                if(massPerVertex.rows() != allVertices.rows() / 3) {
                    throw IllegalArgumentException("There should be equal amounts of all vertices and elements in massPerVertex.");
                }
            }
            
            ObjectProperties(const ObjectProperties& orig)
                    : allVertices(orig.allVertices),
                    surfaceVertexIndices(orig.surfaceVertexIndices),
                    surfaceAreaPerVertex(orig.surfaceAreaPerVertex),
                    massPerVertex(orig.massPerVertex) {}
            
            VectorXf allVertices;
            vector<size_t> surfaceVertexIndices;
            VectorXf surfaceAreaPerVertex;
            VectorXf massPerVertex;
            
            VectorXf getSurfaceVertices() const;
            
            VectorXf mapSurfaceForcesToAllVertices(const VectorXf& surfaceForces) const;
            
            class DensityDistribution {
            private:
                const VectorXf& allVertices;
                const vector<size_t>& surfaceVertexIndices;
                VectorXf massPerVertex;
                
            public:
                DensityDistribution(
                        const VectorXf& allVertices,
                        const vector<size_t>& surfaceVertexIndices,
                        const VectorXf& massPerVertex
                        ) : allVertices(allVertices), surfaceVertexIndices(surfaceVertexIndices),
                                massPerVertex(massPerVertex) {}
                ObjectProperties uniformAreaDistribution(float totalArea);
                ObjectProperties nonUniformAreaDistribution(const VectorXf& surfaceAreaPerVertex);
            };
            
            class UniformTetrahedronDistribution {
            private:
                const VectorXf& allVertices;
                const vector<size_t>& surfaceVertexIndices;
                
            public:
                UniformTetrahedronDistribution(
                        const VectorXf& allVertices,
                        const vector<size_t>& surfaceVertexIndices
                        ) : allVertices(allVertices), surfaceVertexIndices(surfaceVertexIndices) {}
                DensityDistribution uniformDensity(float totalVolume, float density);
                DensityDistribution nonUniformDensity(const VectorXf& massPerVertex);
            };
            
            static UniformTetrahedronDistribution uniformTetrahedronDistribution(const VectorXf& allVertices, const vector<size_t>& surfaceVertexIndices);
            static VectorXf verticesToFlatVector(const engine::renderer::Mesh::ConstVertexProxy& vertices);
            static VectorXf verticesToFlatVector(const vector<engine::renderer::Vertex>& vertices);
        };
    }
}

#endif /* OBJECTPROPERTIES_H */

