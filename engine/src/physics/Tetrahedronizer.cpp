#include "Tetrahedronizer.h"
#include "../IllegalArgumentException.h"

#include "../util/vector.h"
#include "../renderer/Vertex.h"

#include "../util/Set.h"

#include "eigenHashes.h"

using engine::util::vector;
using namespace engine::renderer;

namespace engine {
    namespace physics {
        TetrahedronizedObject Tetrahedronizer::tetrahedronizeCuboid(
                    const Vector3f& corner,
                    const Vector3f& spanVectorOne, const Vector3f& spanVectorTwo, const Vector3f& spanVectorThree,
                    size_t numTetrahedronAlongSpanOne, size_t numTetrahedronAlongSpanTwo, size_t numTetrahedronAlongSpanThree 
                ) {
            if(numTetrahedronAlongSpanOne % 2 != 0
                    || numTetrahedronAlongSpanTwo % 2 != 0
                    || numTetrahedronAlongSpanThree % 2 != 0) {
                throw IllegalArgumentException("The number of tetrahedron needs to be divisible by 2.");
            }
            if(abs(spanVectorOne.cross(spanVectorTwo).dot(spanVectorThree)) < 1e-5) {
                throw IllegalArgumentException("The span vectors must not be in the same plane.");
            }
            
            MatrixXf euclidianToCuboid(3, 3);
            euclidianToCuboid.col(0) = spanVectorOne;
            euclidianToCuboid.col(1) = spanVectorTwo;
            euclidianToCuboid.col(2) = spanVectorThree;
            
            MatrixXf cuboidToEuclidian(euclidianToCuboid.inverse());
            
            float offsetX = 1. / numTetrahedronAlongSpanOne;
            float offsetY = 1. / numTetrahedronAlongSpanTwo;
            float offsetZ = 1. / numTetrahedronAlongSpanThree;
            
            engine::util::vector<size_t> tetrahedronIndices;
            engine::util::vector<Vector3f> allVectors;
            
            engine::util::vector<Vertex> innerVertices;
            engine::util::vector<GLuint> innerFaceIndices;
            engine::util::vector<Vertex> outerVertices;
            engine::util::vector<GLuint> outerFaceIndices;
            
            vector<Array<std::pair<size_t, size_t>>> simulationToRenderVertices;
            
            for(size_t xTetrahedron = 0; xTetrahedron < numTetrahedronAlongSpanOne / 2; ++xTetrahedron) {
                for(size_t yTetrahedron = 0; yTetrahedron < numTetrahedronAlongSpanTwo / 2; ++yTetrahedron) {
                    for(size_t zTetrahedron = 0; zTetrahedron < numTetrahedronAlongSpanThree / 2; ++zTetrahedron) {
                        Vector3f corner_0_0_0 = {xTetrahedron * offsetX, yTetrahedron * offsetY, zTetrahedron * offsetZ};
                        Vector3f corner_1_0_0 = {xTetrahedron * offsetX + offsetX, yTetrahedron * offsetY, zTetrahedron * offsetZ};
                        Vector3f corner_1_0_1 = {xTetrahedron * offsetX + offsetX, yTetrahedron * offsetY, zTetrahedron * offsetZ + offsetZ};
                        Vector3f corner_0_0_1 = {xTetrahedron * offsetX, yTetrahedron * offsetY, zTetrahedron * offsetZ + offsetZ};

                        Vector3f corner_0_1_0 = {xTetrahedron * offsetX, yTetrahedron * offsetY + offsetY, zTetrahedron * offsetZ};
                        Vector3f corner_1_1_0 = {xTetrahedron * offsetX + offsetX, yTetrahedron * offsetY + offsetY, zTetrahedron * offsetZ};
                        Vector3f corner_1_1_1 = {xTetrahedron * offsetX + offsetX, yTetrahedron * offsetY + offsetY, zTetrahedron * offsetZ + offsetZ};
                        Vector3f corner_0_1_1 = {xTetrahedron * offsetX, yTetrahedron * offsetY + offsetY, zTetrahedron * offsetZ + offsetZ};
                        
                        {   // All faces with x == 0
                            auto& veretexList = xTetrahedron == 0 ? outerVertices : innerVertices;
                            auto& indexList = xTetrahedron == 0 ? outerFaceIndices : innerFaceIndices;
                            glm::vec3 color = xTetrahedron == 0 ? vec3(0, 1, 0) : vec3(1, 0, 0);

                            auto firstIndex = veretexList.size();
                            
                            veretexList.push_back(Vertex(corner_0_0_0, color)); // +0
                            veretexList.push_back(Vertex(corner_0_1_0, color)); // +1
                            veretexList.push_back(Vertex(corner_0_1_1, color)); // +2
                            veretexList.push_back(Vertex(corner_0_0_1, color)); // +3
                            
                            indexList.push_back(firstIndex + 0);
                            indexList.push_back(firstIndex + 2);
                            indexList.push_back(firstIndex + 1);
                            
                            indexList.push_back(firstIndex + 0);
                            indexList.push_back(firstIndex + 3);
                            indexList.push_back(firstIndex + 2);
                        }
                        
                        {   // All faces with x == 1
                            auto& veretexList = xTetrahedron+1 == numTetrahedronAlongSpanOne / 2 ? outerVertices : innerVertices;
                            auto& indexList = xTetrahedron+1 == numTetrahedronAlongSpanOne / 2 ? outerFaceIndices : innerFaceIndices;
                            glm::vec3 color = xTetrahedron+1 == numTetrahedronAlongSpanOne / 2 ? vec3(0, 1, 0) : vec3(1, 0, 0);

                            auto firstIndex = veretexList.size();
                            
                            veretexList.push_back(Vertex(corner_1_0_1, color)); // +0
                            veretexList.push_back(Vertex(corner_1_0_0, color)); // +1
                            veretexList.push_back(Vertex(corner_1_1_0, color)); // +2
                            veretexList.push_back(Vertex(corner_1_1_1, color)); // +3
                            
                            indexList.push_back(firstIndex + 0);
                            indexList.push_back(firstIndex + 2);
                            indexList.push_back(firstIndex + 3);
                            
                            indexList.push_back(firstIndex + 0);
                            indexList.push_back(firstIndex + 1);
                            indexList.push_back(firstIndex + 2);
                        }
                        
                        {   // All faces with y == 0
                            auto& veretexList = yTetrahedron == 0 ? outerVertices : innerVertices;
                            auto& indexList = yTetrahedron == 0 ? outerFaceIndices : innerFaceIndices;
                            glm::vec3 color = yTetrahedron == 0 ? vec3(0, 1, 0) : vec3(1, 0, 0);

                            auto firstIndex = veretexList.size();
                            
                            veretexList.push_back(Vertex(corner_0_0_0, color)); // +0
                            veretexList.push_back(Vertex(corner_1_0_0, color)); // +1
                            veretexList.push_back(Vertex(corner_1_0_1, color)); // +2
                            veretexList.push_back(Vertex(corner_0_0_1, color)); // +3
                            
                            indexList.push_back(firstIndex + 0);
                            indexList.push_back(firstIndex + 2);
                            indexList.push_back(firstIndex + 3);
                            
                            indexList.push_back(firstIndex + 0);
                            indexList.push_back(firstIndex + 1);
                            indexList.push_back(firstIndex + 2);
                        }
                        
                        {   // All faces with y == 1
                            auto& veretexList = yTetrahedron+1 == numTetrahedronAlongSpanTwo / 2 ? outerVertices : innerVertices;
                            auto& indexList = yTetrahedron+1 == numTetrahedronAlongSpanTwo / 2 ? outerFaceIndices : innerFaceIndices;
                            glm::vec3 color = yTetrahedron+1 == numTetrahedronAlongSpanTwo / 2 ? vec3(0, 1, 0) : vec3(1, 0, 0);

                            auto firstIndex = veretexList.size();
                            
                            veretexList.push_back(Vertex(corner_0_1_0, color)); // +0
                            veretexList.push_back(Vertex(corner_1_1_0, color)); // +1
                            veretexList.push_back(Vertex(corner_1_1_1, color)); // +2
                            veretexList.push_back(Vertex(corner_0_1_1, color)); // +3
                            
                            indexList.push_back(firstIndex + 0);
                            indexList.push_back(firstIndex + 3);
                            indexList.push_back(firstIndex + 1);
                            
                            indexList.push_back(firstIndex + 2);
                            indexList.push_back(firstIndex + 1);
                            indexList.push_back(firstIndex + 3);
                        }
                        
                        {   // All faces with z == 0
                            auto& veretexList = zTetrahedron == 0 ? outerVertices : innerVertices;
                            auto& indexList = zTetrahedron == 0 ? outerFaceIndices : innerFaceIndices;
                            glm::vec3 color = zTetrahedron == 0 ? vec3(0, 1, 0) : vec3(1, 0, 0);

                            auto firstIndex = veretexList.size();
                            
                            veretexList.push_back(Vertex(corner_0_0_0, color)); // +0
                            veretexList.push_back(Vertex(corner_1_0_0, color)); // +1
                            veretexList.push_back(Vertex(corner_1_1_0, color)); // +2
                            veretexList.push_back(Vertex(corner_0_1_0, color)); // +3
                            
                            indexList.push_back(firstIndex + 0);
                            indexList.push_back(firstIndex + 2);
                            indexList.push_back(firstIndex + 1);
                            
                            indexList.push_back(firstIndex + 0);
                            indexList.push_back(firstIndex + 3);
                            indexList.push_back(firstIndex + 2);
                        }
                        
                        {   // All faces with z == 1
                            auto& veretexList = zTetrahedron+1 == numTetrahedronAlongSpanThree / 2 ? outerVertices : innerVertices;
                            auto& indexList = zTetrahedron+1 == numTetrahedronAlongSpanThree / 2 ? outerFaceIndices : innerFaceIndices;
                            glm::vec3 color = zTetrahedron+1 == numTetrahedronAlongSpanThree / 2 ? vec3(0, 1, 0) : vec3(1, 0, 0);

                            auto firstIndex = veretexList.size();
                            
                            veretexList.push_back(Vertex(corner_0_0_1, color)); // +0
                            veretexList.push_back(Vertex(corner_1_0_1, color)); // +1
                            veretexList.push_back(Vertex(corner_1_1_1, color)); // +2
                            veretexList.push_back(Vertex(corner_0_1_1, color)); // +3
                            
                            indexList.push_back(firstIndex + 0);
                            indexList.push_back(firstIndex + 1);
                            indexList.push_back(firstIndex + 3);
                            
                            indexList.push_back(firstIndex + 1);
                            indexList.push_back(firstIndex + 2);
                            indexList.push_back(firstIndex + 3);
                        }
                        
                        // All purely internal faces
                        auto firstIndex = innerVertices.size();
                        
                        float uvHeight = sqrtf(3) / 3;
                            
                        innerVertices.push_back(Vertex(corner_0_1_1, {1, 0, 0}, {0,    0})); // +0
                        innerVertices.push_back(Vertex(corner_1_0_1, {1, 0, 0}, {1./3, uvHeight})); // +1
                        innerVertices.push_back(Vertex(corner_1_1_0, {1, 0, 0}, {1,    uvHeight})); // +2
                        innerVertices.push_back(Vertex(corner_0_0_0, {1, 0, 0}, {2./3, 0})); // +3

                        innerFaceIndices.push_back(firstIndex + 0);
                        innerFaceIndices.push_back(firstIndex + 1);
                        innerFaceIndices.push_back(firstIndex + 2);

                        innerFaceIndices.push_back(firstIndex + 3);
                        innerFaceIndices.push_back(firstIndex + 1);
                        innerFaceIndices.push_back(firstIndex + 0);

                        innerFaceIndices.push_back(firstIndex + 3);
                        innerFaceIndices.push_back(firstIndex + 0);
                        innerFaceIndices.push_back(firstIndex + 2);

                        innerFaceIndices.push_back(firstIndex + 3);
                        innerFaceIndices.push_back(firstIndex + 2);
                        innerFaceIndices.push_back(firstIndex + 1);
                    }
                }
            }
            
            engine::util::vector<Vector3f> simulationVectors;
            engine::util::Set<Vector3f> deletedVectors;
            deletedVectors.set_empty_key(Vector3f(INFINITY, INFINITY, INFINITY));
            
//            for(size_t i = 0; i < vectors.size(); ++i) {
//                auto& vec = vectors[i];
//                if(deletedVectors.find(vec) == deletedVectors.end()) {
//                    auto newIndex = simulationVectors.size();
//                    
//                    auto euclidianVector = cuboidToEuclidian * vec;
//                    simulationVectors.push_back(Vertex({euclidianVector[0], euclidianVector[1], euclidianVector[2]}, {.9, .9, .9}));
//                    
//                    for(size_t j = i; j < vectors.size(); ++j) {
//                        if(vectors[j].isApprox(vec)) {
//                            // Found the same vector => reduce and update indices
//                            for(auto& ind : faceIndices) {
//                                if(ind == j) {
//                                    ind = newIndex;
//                                }
//                            }
//                            for(auto& ind : tetrahedronIndices) {
//                                if(ind == j) {
//                                    ind = newIndex;
//                                }
//                            }
//                        }
//                    }
//                    
//                    deletedVectors.insert(vec);
//                }
//            }
            
            VectorXf simulationMesh(simulationVectors.size() * 3);
            int i = 0;
            for(auto& v : simulationVectors) {
                simulationMesh[i++] = v[0];
                simulationMesh[i++] = v[1];
                simulationMesh[i++] = v[2];
            }
            
            vector<std::shared_ptr<Mesh>> renderMeshes;
            renderMeshes.reserve(2);
            renderMeshes.push_back(std::make_shared<Mesh>(outerVertices, outerFaceIndices, DataUsagePattern::DYNAMIC_DRAW));
            renderMeshes.push_back(std::make_shared<Mesh>(innerVertices, innerFaceIndices, DataUsagePattern::DYNAMIC_DRAW));
            
            ObjectProperties properties(simulationMesh, {}, VectorXf(0), VectorXf(0));
            return TetrahedronizedObject(simulationMesh, renderMeshes, simulationToRenderVertices, properties, tetrahedronIndices);
        }
    }
}