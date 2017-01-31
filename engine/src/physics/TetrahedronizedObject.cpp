#include "TetrahedronizedObject.h"
#include "IllegalArgumentException.h"

#include <glm/gtx/norm.hpp>

namespace engine {
    namespace physics {
        TetrahedronizedObject::TetrahedronizedObject(
                const VectorXf& simulationMesh,
                const vector<size_t>& surfaceIndices,
                const float density,
                const vector<std::shared_ptr<Mesh>>& meshes,
                const vector<Array<std::pair<size_t, size_t>>>& simulationToRenderVertices,
                const Array<size_t>& tetrahedronIndices)
            : SimulationObject(simulationMesh, meshes, simulationToRenderVertices), tetrahedronIndices(tetrahedronIndices) {
            if(tetrahedronIndices.size() % 4 != 0) {
                throw IllegalArgumentException("The list of tetrahedron indices should be divisable by 4! Size: %zu", tetrahedronIndices.size());
            }
            LOG(INFO) << "Calculating properties of deformable object...";
            this->properties = std::make_unique<ObjectProperties>(
                    this->simulationMesh,
                    surfaceIndices,
                    this->calculateSurfaceAreas(0, surfaceIndices),
                    this->calculateMasses(density));
            LOG(INFO) << "Done";
        }
        TetrahedronizedObject::TetrahedronizedObject(const TetrahedronizedObject& orig)
            : SimulationObject(orig), properties(std::make_unique<ObjectProperties>(*orig.properties)), tetrahedronIndices(orig.tetrahedronIndices) {}
        
        TetrahedronizedObject::TetrahedronizedObject(TetrahedronizedObject&& orig)
            : SimulationObject(orig), properties(std::move(orig.properties)),
                tetrahedronIndices(std::move(orig.tetrahedronIndices)) {}
        
        TetrahedronizedObject& TetrahedronizedObject::operator=(const TetrahedronizedObject& orig) {
            SimulationObject::operator=(orig);
            this->properties = std::make_unique<ObjectProperties>(*orig.properties);
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
            return *this->properties;
        }
        const ObjectProperties& TetrahedronizedObject::getProperties() const {
            return *this->properties;
        }
        
        VectorXf TetrahedronizedObject::calculateSurfaceAreas(size_t surfaceMeshIndex, const vector<size_t>& surfaceVertexIndices) const {
            VectorXf surfaceAreas = VectorXf::Zero(surfaceVertexIndices.size());
            int surfaceIndex = 0;
            for(auto vertIndex : surfaceVertexIndices) {
                auto& meshVertexIndices = this->simulationToRenderVertices[vertIndex];
                
                float sumArea = 0;
                for(auto& meshVertexIndex : meshVertexIndices) {
                    if(meshVertexIndex.first != surfaceMeshIndex) {
                        continue;
                    }
                    const auto& mesh = this->renderMeshes[surfaceMeshIndex];
                    for(size_t faceIndex = 0; faceIndex < mesh->getFaceIndices().size(); faceIndex += 3) {
                        bool isRelevant = false;
                        for(size_t i = 0; i < 3; ++i) {
                            if(mesh->getFaceIndices()[faceIndex+i] == meshVertexIndex.second) {
                                isRelevant = true;
                                break;
                            }
                        }
                        if(isRelevant) {
                            const auto& v1 = mesh->getVertices()[mesh->getFaceIndices()[faceIndex+0]].position;
                            const auto& v2 = mesh->getVertices()[mesh->getFaceIndices()[faceIndex+1]].position;
                            const auto& v3 = mesh->getVertices()[mesh->getFaceIndices()[faceIndex+2]].position;
                            sumArea += (glm::length(glm::cross(v2 - v1, v3 - v1)) / 2.) / 3.;
                        }
                    }
                }
                surfaceAreas[surfaceIndex++] = sumArea;
            }
            return surfaceAreas;
        }
        
        VectorXf TetrahedronizedObject::calculateMasses(float density) const {
            VectorXf masses = VectorXf::Zero(this->simulationMesh.rows() / 3);
            for(int vertIndex = 0; vertIndex < this->simulationMesh.rows() / 3; ++vertIndex) {
                float sumMass = 0;
                
                for(size_t tetrahedronIndex = 0; tetrahedronIndex < this->getNumberOfTetrahedron(); ++tetrahedronIndex) {
                    bool isRelevant = false;
                    for(size_t i = 0; i < 4; ++i) {
                        if(static_cast<size_t>(vertIndex) == this->getIndexOfVertexInTetrahedron(tetrahedronIndex, i)) {
                            isRelevant = true;
                            break;
                        }
                    }
                    if(isRelevant) {
                        sumMass += this->calculateVolumeOfTetrahedron(tetrahedronIndex) * density / 4;
                    }
                }
                
                masses[vertIndex] = sumMass;
            }
            return masses;
        }
    }
}
