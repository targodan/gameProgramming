#include "ObjectProperties.h"

namespace engine {
    namespace physics {
        Matrix<float, Dynamic, 1> ObjectProperties::mapSurfaceForcesToAllVertices(const Matrix<float, Dynamic, 1>& surfaceForces) const {
            Matrix<float, Dynamic, 1> forces = Matrix<float, Dynamic, 1>::Zero(this->allVertices.rows(), 1);
            size_t i = 0;
            for(auto index : this->surfaceVertexIndices) {
                forces(index + 0) = surfaceForces(i + 0);
                forces(index + 1) = surfaceForces(i + 1);
                forces(index + 2) = surfaceForces(i + 2);
                i += 3;
            }
            return forces;
        }


        ObjectProperties ObjectProperties::DensityDistribution::uniformAreaDistribution(float totalArea) {
            auto numVertices = this->allVertices.rows() / 3;
            auto areaPerVertex = MatrixXf::Ones(numVertices, 1);
            return ObjectProperties(this->allVertices, this->surfaceVertexIndices, this->massPerVertex, areaPerVertex * (totalArea / numVertices));
        }
        ObjectProperties ObjectProperties::DensityDistribution::nonUniformAreaDistribution(const Matrix<float, Dynamic, 1>& surfaceAreaPerVertex) {
            return ObjectProperties(this->allVertices, this->surfaceVertexIndices, this->massPerVertex, surfaceAreaPerVertex);
        }

        ObjectProperties::DensityDistribution ObjectProperties::UniformTetrahedronDistribution::uniformDensity(float totalVolume, float density) {
            auto numVertices = this->allVertices.rows() / 3;
            auto massPerVertex = MatrixXf::Ones(numVertices, 1);
            return DensityDistribution(this->allVertices, this->surfaceVertexIndices, massPerVertex * (totalVolume * density / numVertices));
        }
        ObjectProperties::DensityDistribution ObjectProperties::UniformTetrahedronDistribution::nonUniformDensity(const Matrix<float, Dynamic, 1>& massPerVertex) {
            return DensityDistribution(this->allVertices, this->surfaceVertexIndices, massPerVertex);
        }

        ObjectProperties::UniformTetrahedronDistribution ObjectProperties::uniformTetrahedronDistribution(const Matrix<float, Dynamic, 1>& allVertices, const vector<size_t>& surfaceVertexIndices) {
            return UniformTetrahedronDistribution(allVertices, surfaceVertexIndices);
        }
    }
}