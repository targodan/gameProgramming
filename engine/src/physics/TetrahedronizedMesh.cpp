#include "TetrahedronizedMesh.h"
#include "IllegalArgumentException.h"

namespace engine {
    namespace physics {
        TetrahedronizedMesh::TetrahedronizedMesh(Mesh& mesh, const std::initializer_list<size_t>& tetrahedronIndices)
            : mesh(mesh), tetrahedronIndices(tetrahedronIndices) {
            if(tetrahedronIndices.size() % 4 != 0) {
                throw IllegalArgumentException("The list of tetrahedron indices should be divisable by 4! Size: %zu", tetrahedronIndices.size());
            }
        }
        TetrahedronizedMesh::TetrahedronizedMesh(Mesh& mesh, const Array<size_t>& tetrahedronIndices)
            : mesh(mesh), tetrahedronIndices(tetrahedronIndices) {
            if(tetrahedronIndices.size() % 4 != 0) {
                throw IllegalArgumentException("The list of tetrahedron indices should be divisable by 4! Size: %zu", tetrahedronIndices.size());
            }
        }
        TetrahedronizedMesh::TetrahedronizedMesh(const TetrahedronizedMesh& orig)
            : mesh(orig.mesh), tetrahedronIndices(orig.tetrahedronIndices) {}

//        TetrahedronizedMesh& TetrahedronizedMesh::operator=(TetrahedronizedMesh&& orig) {
//            std::swap(this->mesh, orig.mesh);
//            std::swap(this->tetrahedronIndices, orig.tetrahedronIndices);
//            return *this;
//        }
        
        Vector3f TetrahedronizedMesh::getVertex(size_t index) const {
            const auto& vertex = this->mesh.getVertices()[index];
            return Vector3f(
                    vertex.position.x,
                    vertex.position.y,
                    vertex.position.z
                );
        }
        
        MatrixXf TetrahedronizedMesh::getTetrahedron(size_t index) const {
            MatrixXf tetrahedron(3, 4);
            tetrahedron.col(0) = this->getVertex(this->tetrahedronIndices[index * 4 + 0]);
            tetrahedron.col(1) = this->getVertex(this->tetrahedronIndices[index * 4 + 1]);
            tetrahedron.col(2) = this->getVertex(this->tetrahedronIndices[index * 4 + 2]);
            tetrahedron.col(3) = this->getVertex(this->tetrahedronIndices[index * 4 + 3]);
            return tetrahedron;
        }
        
        VectorXf TetrahedronizedMesh::getTetrahedronPlanar(size_t index) const {
            VectorXf tetrahedron(12);
            tetrahedron <<
                    this->getVertex(this->tetrahedronIndices[index * 4 + 0]),
                    this->getVertex(this->tetrahedronIndices[index * 4 + 1]),
                    this->getVertex(this->tetrahedronIndices[index * 4 + 2]),
                    this->getVertex(this->tetrahedronIndices[index * 4 + 3]);
            return tetrahedron;
        }
        
        void TetrahedronizedMesh::updateMeshFromPlanarVector(const VectorXf& vertices) {
            for(size_t i = 0; i < 12; i += 3) {
                this->mesh.getVertices()[i / 3].position.x = vertices[i+0];
                this->mesh.getVertices()[i / 3].position.y = vertices[i+1];
                this->mesh.getVertices()[i / 3].position.z = vertices[i+2];
            }
            this->mesh.setVerticesChanged(true);
        }
        
        float TetrahedronizedMesh::calculateVolumeOfTetrahedron(size_t index) const {
            MatrixXf tetrahedron = this->getTetrahedron(index);
            
            Vector3f normal = (static_cast<Vector3f>(tetrahedron.col(1) - tetrahedron.col(0)))
                                .cross
                              (static_cast<Vector3f>(tetrahedron.col(2) - tetrahedron.col(0)));
            
            float normalLength = normal.norm();
            float baseArea = normalLength / 2.;
            normal *= 1. / normalLength;
            
            Hyperplane<float, 3> basePlane(normal, tetrahedron.col(0));
            return baseArea * basePlane.absDistance(tetrahedron.col(3)) / 3.;
        }
        
        float TetrahedronizedMesh::calculateVolume() const {
            float volume = 0;
            for(size_t i = 0; i < this->tetrahedronIndices.size() / 4; ++i) {
                volume += this->calculateVolumeOfTetrahedron(i);
            }
            return volume;
        }
    }
}
