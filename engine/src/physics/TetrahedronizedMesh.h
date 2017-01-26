#ifndef TETRAHEDRONIZEDMESH_H
#define TETRAHEDRONIZEDMESH_H

#include <initializer_list>
#include <eigen3/Eigen/Eigen>
#include <memory>

#include "../util/Array.h"
#include "../renderer/Mesh.h"

namespace engine {
    namespace physics {
        using Eigen::VectorXf;
        using Eigen::Vector3f;
        using Eigen::MatrixXf;
        using Eigen::Hyperplane;
        
        using engine::renderer::Mesh;
        using engine::util::Array;
        
        class TetrahedronizedMesh {
        private:
            std::shared_ptr<Mesh> mesh;
            Array<size_t> tetrahedronIndices;
            
        public:
            TetrahedronizedMesh(const std::shared_ptr<Mesh>& mesh, const std::initializer_list<size_t>& tetrahedronIndices);
            TetrahedronizedMesh(const std::shared_ptr<Mesh>& mesh, const Array<size_t>& tetrahedronIndices);
            TetrahedronizedMesh(const TetrahedronizedMesh& orig);
            
            // This would not work, as we cannot make the mesh reference point to a different mesh
            TetrahedronizedMesh& operator=(const TetrahedronizedMesh& orig) = delete;
            TetrahedronizedMesh& operator=(TetrahedronizedMesh&& orig) = delete;
            
            Mesh& getMesh();
            const Mesh& getMesh() const;
            
            std::shared_ptr<Mesh> getMeshPtr();
            
            size_t getNumberOfTetrahedron() const;
            
            const Array<size_t>& getTetrahedronIndices() const;
            Array<size_t>& getTetrahedronIndices();
            
            size_t getIndexOfVertexInTetrahedron(size_t tetrahedronIndex, size_t vertexIndex) const;
            
            void updateMeshFromPlanarVector(const VectorXf& vertices);
            
            Vector3f getVertex(size_t index) const;
            MatrixXf getTetrahedron(size_t index) const;
            VectorXf getTetrahedronPlanar(size_t index) const;
            
            float calculateVolumeOfTetrahedron(size_t index) const;
            float calculateVolume() const;
        };
    }
}

#endif /* TETRAHEDRONIZEDMESH_H */

