#include "SimulationObject.h"

namespace engine {
    namespace physics {
        SimulationObject::SimulationObject(
                const VectorXf& simulationMesh,
                const vector<std::shared_ptr<Mesh>>& meshes,
                const vector<Array<std::pair<size_t, size_t>>>& simulationToRenderVertices
            ) : simulationMesh(simulationMesh), renderMeshes(meshes),
                simulationToRenderVertices(simulationToRenderVertices) {}
        
        SimulationObject::SimulationObject(const SimulationObject& orig)
            : simulationMesh(orig.simulationMesh), renderMeshes(orig.renderMeshes), simulationToRenderVertices(orig.simulationToRenderVertices) {}
        SimulationObject::SimulationObject(SimulationObject&& orig)
            : simulationMesh(std::move(orig.simulationMesh)), renderMeshes(std::move(orig.renderMeshes)), simulationToRenderVertices(std::move(orig.simulationToRenderVertices)) {}

        SimulationObject& SimulationObject::operator=(const SimulationObject& orig) {
            this->simulationMesh = orig.simulationMesh;
            this->renderMeshes = orig.renderMeshes;
            this->simulationToRenderVertices = orig.simulationToRenderVertices;
            
            return *this;
        }
        SimulationObject& SimulationObject::operator=(SimulationObject&& orig) {
            std::swap(this->simulationMesh, orig.simulationMesh);
            std::swap(this->renderMeshes, orig.renderMeshes);
            std::swap(this->simulationToRenderVertices, orig.simulationToRenderVertices);
            
            return *this;
        }
        
        const VectorXf& SimulationObject::getSimulationMesh() const {
            return this->simulationMesh;
        }
        
        VectorXf& SimulationObject::getSimulationMesh() {
            return this->simulationMesh;
        }
        
        Vector3f SimulationObject::getVertexOfSimulationMesh(size_t index) const {
            return this->simulationMesh.segment<3>(index * 3);
        }
        
        Mesh& SimulationObject::getMesh(size_t i) {
            return *this->renderMeshes[i];
        }
        const Mesh& SimulationObject::getMesh(size_t i) const {
            return *this->renderMeshes[i];
        }
        
        std::shared_ptr<Mesh> SimulationObject::getMeshPtr(size_t i) {
            return this->renderMeshes[i];
        }
        
        void SimulationObject::updateMeshFromPlanarVector(const VectorXf& vertices) {
            this->simulationMesh = vertices;
            this->updateRenderMeshes();
        }
        
        void SimulationObject::updateMeshFromPlanarVector(const VectorXd& vertices) {
            this->simulationMesh = vertices.cast<float>();
            this->updateRenderMeshes();
        }
        
        void SimulationObject::updateRenderMeshes() {
            for(int planarVertexIndex = 0; planarVertexIndex < this->simulationMesh.rows(); planarVertexIndex += 3) {
                for(auto meshVertexIndex : this->simulationToRenderVertices[planarVertexIndex / 3]) {
                    this->renderMeshes[meshVertexIndex.first]->getVertices()[meshVertexIndex.second].position.x = this->simulationMesh[planarVertexIndex+0];
                    this->renderMeshes[meshVertexIndex.first]->getVertices()[meshVertexIndex.second].position.y = this->simulationMesh[planarVertexIndex+1];
                    this->renderMeshes[meshVertexIndex.first]->getVertices()[meshVertexIndex.second].position.z = this->simulationMesh[planarVertexIndex+2];
                    this->renderMeshes[meshVertexIndex.first]->setVerticesChanged(true);
                }
            }
        }
    }
}