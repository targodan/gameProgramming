#ifndef OBJECTPROPERTIES_H
#define OBJECTPROPERTIES_H

#include <eigen3/Eigen/Eigen>

#include "../util/vector.h"

namespace engine {
    namespace physics {
        using namespace Eigen;
        using engine::util::vector;
        
        struct ObjectProperties {
            ObjectProperties(const Matrix<float, Dynamic, 1>& allVertices,
                    const vector<size_t>& surfaceVertexIndices,
                    const Matrix<float, Dynamic, 1>& surfaceAreaPerVertex,
                    const Matrix<float, Dynamic, 1>& massPerVertex)
                    : allVertices(allVertices), surfaceVertexIndices(surfaceVertexIndices),
                    surfaceVertices(surfaceVertexIndices.size() * 3, 1),
                    surfaceAreaPerVertex(surfaceAreaPerVertex),
                    massPerVertex(massPerVertex) {
                size_t i = 0;
                for(auto index : surfaceVertexIndices) {
                    this->surfaceVertices(i++) = this->allVertices(index * 3 + 0);
                    this->surfaceVertices(i++) = this->allVertices(index * 3 + 1);
                    this->surfaceVertices(i++) = this->allVertices(index * 3 + 2);
                }
            }
            
            Matrix<float, Dynamic, 1> allVertices;
            vector<size_t> surfaceVertexIndices;
            Matrix<float, Dynamic, 1> surfaceVertices;
            Matrix<float, Dynamic, 1> surfaceAreaPerVertex;
            Matrix<float, Dynamic, 1> massPerVertex;
            
            Matrix<float, Dynamic, 1> mapSurfaceForcesToAllVertices(const Matrix<float, Dynamic, 1>& surfaceForces) const;
            
            class DensityDistribution {
            private:
                const Matrix<float, Dynamic, 1>& allVertices;
                const vector<size_t>& surfaceVertexIndices;
                Matrix<float, Dynamic, 1> massPerVertex;
                
            public:
                DensityDistribution(
                        const Matrix<float, Dynamic, 1>& allVertices,
                        const vector<size_t>& surfaceVertexIndices,
                        const Matrix<float, Dynamic, 1>& massPerVertex
                        ) : allVertices(allVertices), surfaceVertexIndices(surfaceVertexIndices),
                                massPerVertex(massPerVertex) {}
                ObjectProperties uniformAreaDistribution(float totalArea);
                ObjectProperties nonUniformAreaDistribution(const Matrix<float, Dynamic, 1>& surfaceAreaPerVertex);
            };
            
            class UniformTetrahedronDistribution {
            private:
                const Matrix<float, Dynamic, 1>& allVertices;
                const vector<size_t>& surfaceVertexIndices;
                
            public:
                UniformTetrahedronDistribution(
                        const Matrix<float, Dynamic, 1>& allVertices,
                        const vector<size_t>& surfaceVertexIndices
                        ) : allVertices(allVertices), surfaceVertexIndices(surfaceVertexIndices) {}
                DensityDistribution uniformDensity(float totalVolume, float density);
                DensityDistribution nonUniformDensity(const Matrix<float, Dynamic, 1>& massPerVertex);
            };
            
            static UniformTetrahedronDistribution uniformTetrahedronDistribution(const Matrix<float, Dynamic, 1>& allVertices, const vector<size_t>& surfaceVertexIndices);
        };
    }
}

#endif /* OBJECTPROPERTIES_H */

