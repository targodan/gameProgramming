#ifndef TETRAHEDRONIZEDMESH_H
#define TETRAHEDRONIZEDMESH_H

#include <initializer_list>
#include <eigen3/Eigen/Eigen>
#include <memory>

#include "ObjectProperties.h"
#include "../util/Array.h"
#include "../renderer/Mesh.h"

#include "SimulationObject.h"

namespace engine {
    namespace physics {
        using Eigen::VectorXf;
        using Eigen::Vector3f;
        using Eigen::MatrixXf;
        using Eigen::Hyperplane;
        
        using engine::renderer::Mesh;
        using engine::util::Array;
        
        class TetrahedronizedObject : public SimulationObject {
        protected:
            std::unique_ptr<ObjectProperties> properties;
            Array<size_t> tetrahedronIndices;
            
            VectorXf calculateSurfaceAreas(size_t surfaceMeshIndex, const vector<size_t>& surfaceVertexIndices) const;
            VectorXf calculateMasses(float density) const;
            
        public:
            TetrahedronizedObject(
                    const VectorXf& simulationMesh,
                    const vector<size_t>& surfaceIndices,
                    const float density,
                    const vector<std::shared_ptr<Mesh>>& meshes,
                    const vector<Array<std::pair<size_t, size_t>>>& simulationToRenderVertices,
                    const Array<size_t>& tetrahedronIndices);
            TetrahedronizedObject(const TetrahedronizedObject& orig);
            TetrahedronizedObject(TetrahedronizedObject&& orig);
            
            TetrahedronizedObject& operator=(const TetrahedronizedObject& orig);
            TetrahedronizedObject& operator=(TetrahedronizedObject&& orig);
            
            ObjectProperties& getProperties();
            const ObjectProperties& getProperties() const;
            
            size_t getNumberOfTetrahedron() const;
            
            const Array<size_t>& getTetrahedronIndices() const;
            Array<size_t>& getTetrahedronIndices();
            
            size_t getIndexOfVertexInTetrahedron(size_t tetrahedronIndex, size_t vertexIndex) const;
            
            MatrixXf getTetrahedron(size_t index) const;
            VectorXf getTetrahedronPlanar(size_t index) const;
            
            float calculateVolumeOfTetrahedron(size_t index) const;
            float calculateVolume() const;
        };
    }
}

#endif /* TETRAHEDRONIZEDMESH_H */

