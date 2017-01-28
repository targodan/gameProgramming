#ifndef SIMULATIONMESH_H
#define SIMULATIONMESH_H

#include <eigen3/Eigen/Eigen>
#include <memory>
#include <initializer_list>

#include "../renderer/Mesh.h"
#include "../util/vector.h"
#include "../util/Array.h"

namespace engine {
    namespace physics {
        using Eigen::VectorXf;
        using Eigen::Vector3f;
        using Eigen::MatrixXf;
        using engine::renderer::Mesh;
        using engine::util::vector;
        using engine::util::Array;
        
        class SimulationObject {
        protected:
            VectorXf simulationMesh;
            vector<std::shared_ptr<Mesh>> renderMeshes;
            vector<Array<std::pair<size_t, size_t>>> simulationToRenderVertices;
            
            void updateRenderMeshes();
            
        public:
            SimulationObject(
                    const VectorXf& simulationMesh,
                    const vector<std::shared_ptr<Mesh>>& meshes,
                    const vector<Array<std::pair<size_t, size_t>>>& simulationToRenderVertices
                );
            SimulationObject(const SimulationObject& orig);
            SimulationObject(SimulationObject&& orig);
            
            SimulationObject& operator=(const SimulationObject& orig);
            SimulationObject& operator=(SimulationObject&& orig);
            
            const VectorXf& getSimulationMesh() const;
            Vector3f getVertexOfSimulationMesh(size_t index) const;
            
            Mesh& getMesh(size_t i);
            const Mesh& getMesh(size_t i) const;
            
            std::shared_ptr<Mesh> getMeshPtr(size_t i);
            
            void updateMeshFromPlanarVector(const VectorXf& vertices);
        };
    }
}

#endif /* SIMULATIONMESH_H */

