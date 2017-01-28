#include "TetrahedronizedObject.h"
#include "IllegalArgumentException.h"

namespace engine {
    namespace physics {
        TetrahedronizedObject::TetrahedronizedObject(
                const VectorXf& simulationMesh,
                const vector<std::shared_ptr<Mesh>>& meshes,
                const vector<Array<std::pair<size_t, size_t>>>& simulationToRenderVertices,
                const ObjectProperties& properties,
                const Array<size_t>& tetrahedronIndices)
            : SimulationObject(meshes, simulationMesh, simulationToRenderVertices), properties(properties), tetrahedronIndices(tetrahedronIndices) {
            if(tetrahedronIndices.size() % 4 != 0) {
                throw IllegalArgumentException("The list of tetrahedron indices should be divisable by 4! Size: %zu", tetrahedronIndices.size());
            }
        }
        TetrahedronizedObject::TetrahedronizedObject(const TetrahedronizedObject& orig)
            : SimulationObject(orig), properties(orig.properties), tetrahedronIndices(orig.tetrahedronIndices) {}
        
        TetrahedronizedObject::TetrahedronizedObject(TetrahedronizedObject&& orig)
            : SimulationObject(orig), properties(std::move(orig.properties)),
                tetrahedronIndices(std::move(orig.tetrahedronIndices)) {}
        
        TetrahedronizedObject& TetrahedronizedObject::operator=(const TetrahedronizedObject& orig) {
            SimulationObject::operator=(orig);
            this->properties = orig.properties;
            this->tetrahedronIndices = orig.tetrahedronIndices;
            
            return *this;
        }
        TetrahedronizedObject& TetrahedronizedObject::operator=(TetrahedronizedObject&& orig) {
            SimulationObject::operator=(orig);
            std::swap(this->properties, orig.properties);
            std::swap(this->tetrahedronIndices, orig.tetrahedronIndices);
            
            return *this;
        }
        
        
        MatrixXf TetrahedronizedObject::getTetrahedron(size_t index) const {
            MatrixXf tetrahedron(3, 4);
            tetrahedron.col(0) = this->getVertexOfSimulationMesh(this->tetrahedronIndices[index * 4 + 0]);
            tetrahedron.col(1) = this->getVertexOfSimulationMesh(this->tetrahedronIndices[index * 4 + 1]);
            tetrahedron.col(2) = this->getVertexOfSimulationMesh(this->tetrahedronIndices[index * 4 + 2]);
            tetrahedron.col(3) = this->getVertexOfSimulationMesh(this->tetrahedronIndices[index * 4 + 3]);
            return tetrahedron;
        }
        
        VectorXf TetrahedronizedObject::getTetrahedronPlanar(size_t index) const {
            VectorXf tetrahedron(12);
            tetrahedron <<
                    this->getVertexOfSimulationMesh(this->tetrahedronIndices[index * 4 + 0]),
                    this->getVertexOfSimulationMesh(this->tetrahedronIndices[index * 4 + 1]),
                    this->getVertexOfSimulationMesh(this->tetrahedronIndices[index * 4 + 2]),
                    this->getVertexOfSimulationMesh(this->tetrahedronIndices[index * 4 + 3]);
            return tetrahedron;
        }
        
        float TetrahedronizedObject::calculateVolumeOfTetrahedron(size_t index) const {
            MatrixXf tetrahedron = this->getTetrahedron(index);
            
            return  abs(
                        (static_cast<Vector3f>(tetrahedron.col(1) - tetrahedron.col(0)))
                        .cross
                        (static_cast<Vector3f>(tetrahedron.col(2) - tetrahedron.col(0)))
                        .dot(static_cast<Vector3f>(tetrahedron.col(3) - tetrahedron.col(0)))
                    ) / 6;
        }
        
        float TetrahedronizedObject::calculateVolume() const {
            float volume = 0;
            for(size_t i = 0; i < this->tetrahedronIndices.size() / 4; ++i) {
                volume += this->calculateVolumeOfTetrahedron(i);
            }
            return volume;
        }
        
        size_t TetrahedronizedObject::getNumberOfTetrahedron() const {
            return this->tetrahedronIndices.size() / 4;
        }

        const Array<size_t>& TetrahedronizedObject::getTetrahedronIndices() const {
            return this->tetrahedronIndices;
        }

        Array<size_t>& TetrahedronizedObject::getTetrahedronIndices() {
            return this->tetrahedronIndices;
        }

        size_t TetrahedronizedObject::getIndexOfVertexInTetrahedron(size_t tetrahedronIndex, size_t vertexIndex) const {
            return this->tetrahedronIndices[tetrahedronIndex * 4 + vertexIndex];
        }

        ObjectProperties& TetrahedronizedObject::getProperties() {
            return this->properties;
        }
        const ObjectProperties& TetrahedronizedObject::getProperties() const {
            return this->properties;
        }
    }
}
