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
           
            virtual Mesh* clone() override {
                return new InstanceMesh(*this);
            };
            
            virtual void loadMesh() override;
            virtual void render() override;
            
            const vector<float>& getInstancePositions() const;
            vector<float>& getInstancePositions();
            void setInstancePositions(const vector<float>& instanceCentres);
            
            void setInstancePositionsChanged(bool changed);
        protected:
            virtual void setVAOAttributes() override;
            void setVAODivisor();
            void createPositionVBO(vector<float>& positions, DataUsagePattern usage);
            
            vector<float> instancePositions;
            bool instancePositionsChanged = false;
            bool createdBuffer = false;
            
            DataUsagePattern usage;
        };
    }
}


#endif /* INSTANCEMESH_H */

