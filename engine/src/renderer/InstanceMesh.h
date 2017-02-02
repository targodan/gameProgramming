#ifndef INSTANCEMESH_H
#define INSTANCEMESH_H

#include "Mesh.h"
#include "Vertex.h"
#include "DataUsagePattern.h"
#include "../util/vector.h"

namespace engine {
    namespace renderer {
        class InstanceMesh : public Mesh {
        public:
            InstanceMesh(const vector<Vertex>& vertices, const vector<float>& instanceCentres, DataUsagePattern usage = DataUsagePattern::STREAM_DRAW);
            InstanceMesh(const InstanceMesh& orig);
            InstanceMesh(InstanceMesh&& orig);
            InstanceMesh& operator=(const InstanceMesh& right);
            InstanceMesh& operator=(InstanceMesh&& right);
            virtual ~InstanceMesh();
           
            virtual void loadMesh() override;
            virtual void render() override;
            
            // virtual void setMaterial(const std::shared_ptr<Material>& material) override;
            
            const vector<float>& getInstancePositions() const;
            vector<float>& getInstancePositions();
            void setInstancePositions(const vector<float>& instanceCentres);
            
            void setInstancePositionsChanged(bool changed);
        protected:
            virtual void setVAOAttributes() override;
            void setVAODivisor();
            void createVBO(vector<float>& positions, DataUsagePattern usage);
            
            vector<float> instancePositions;
            bool instancePositionsChanged = false;
            
            DataUsagePattern usage;
        };
    }
}


#endif /* INSTANCEMESH_H */

