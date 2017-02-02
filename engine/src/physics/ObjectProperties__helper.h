#ifndef SIMULATIONMESH__HELPER_H
#define SIMULATIONMESH__HELPER_H

// This is inside the ObjectProperties.

// struct ObjectProperties {

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

// }

#endif /* SIMULATIONMESH__HELPER_H */

