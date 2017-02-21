#ifndef FREEZER_H
#define FREEZER_H

#include <math.h>

#include "engine/physics/DeformableBody.h"
#include "engine/physics/TetrahedronizedObject.h"

void freezeAllWith(engine::physics::DeformableBody& defBody, engine::physics::TetrahedronizedObject& tMesh, float x = NAN, float y = NAN, float z = NAN) {
    const auto& sMesh = tMesh.getSimulationMesh();

    engine::util::vector<size_t> freezer;

    for(int vertexInd = 0; vertexInd < sMesh.rows() / 3; ++vertexInd) {
        if(        (isnan(x) || abs(x - sMesh[vertexInd * 3 + 0]) < 1e-3)
                && (isnan(y) || abs(y - sMesh[vertexInd * 3 + 1]) < 1e-3)
                && (isnan(z) || abs(z - sMesh[vertexInd * 3 + 2]) < 1e-3)) {
            freezer.push_back(vertexInd);
        }
    }

    defBody.freezeVertices(freezer);
}
void unfreezeAllWith(engine::physics::DeformableBody& defBody, engine::physics::TetrahedronizedObject& tMesh, float x = NAN, float y = NAN, float z = NAN) {
    const auto& sMesh = tMesh.getSimulationMesh();

    engine::util::vector<size_t> freezer;

    for(int vertexInd = 0; vertexInd < sMesh.rows() / 3; ++vertexInd) {
        if(        (isnan(x) || abs(x - sMesh[vertexInd * 3 + 0]) < 1e-3)
                && (isnan(y) || abs(y - sMesh[vertexInd * 3 + 1]) < 1e-3)
                && (isnan(z) || abs(z - sMesh[vertexInd * 3 + 2]) < 1e-3)) {
            freezer.push_back(vertexInd);
        }
    }

    defBody.unfreezeVertices(freezer);
}


#endif /* FREEZER_H */

