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
            tetrahedronIndices.reserve(
                    numTetrahedronAlongSpanOne * 4
                    + numTetrahedronAlongSpanTwo * 4
                    + numTetrahedronAlongSpanThree * 4
                );
            engine::util::vector<Vector3f> vectors;
            vectors.reserve(numTetrahedronAlongSpanOne * numTetrahedronAlongSpanTwo * numTetrahedronAlongSpanThree);
            
            engine::util::vector<GLuint> faceIndices;
            faceIndices.reserve(vectors.size() * 3 * 4);
            
            for(size_t xTetrahedron = 0; xTetrahedron < numTetrahedronAlongSpanOne / 2; ++xTetrahedron) {
                for(size_t yTetrahedron = 0; yTetrahedron < numTetrahedronAlongSpanTwo / 2; ++yTetrahedron) {
                    for(size_t zTetrahedron = 0; zTetrahedron < numTetrahedronAlongSpanThree / 2; ++zTetrahedron) {
                        size_t first = vectors.size();
                        vectors.push_back({xTetrahedron * offsetX, yTetrahedron * offsetY, zTetrahedron * offsetZ});
                        vectors.push_back({xTetrahedron * offsetX + offsetX, yTetrahedron * offsetY, zTetrahedron * offsetZ});
                        vectors.push_back({xTetrahedron * offsetX, yTetrahedron * offsetY + offsetY, zTetrahedron * offsetZ});
                        vectors.push_back({xTetrahedron * offsetX, yTetrahedron * offsetY, zTetrahedron * offsetZ + offsetZ});

                        vectors.push_back({xTetrahedron * offsetX, yTetrahedron * offsetY + offsetY, zTetrahedron * offsetZ + offsetZ});
                        vectors.push_back({xTetrahedron * offsetX + offsetX, yTetrahedron * offsetY + offsetY, zTetrahedron * offsetZ + offsetZ});
                        vectors.push_back({xTetrahedron * offsetX + offsetX, yTetrahedron * offsetY, zTetrahedron * offsetZ + offsetZ});
                        vectors.push_back({xTetrahedron * offsetX + offsetX, yTetrahedron * offsetY + offsetY, zTetrahedron * offsetZ});
                        
                        
                        faceIndices.push_back(first + 3);
                        faceIndices.push_back(first + 1);
                        faceIndices.push_back(first + 0);
                        
                        faceIndices.push_back(first + 3);
                        faceIndices.push_back(first + 6);
                        faceIndices.push_back(first + 1);
                        
                        faceIndices.push_back(first + 3);
                        faceIndices.push_back(first + 6);
                        faceIndices.push_back(first + 5);
                        
                        faceIndices.push_back(first + 3);
                        faceIndices.push_back(first + 5);
                        faceIndices.push_back(first + 4);
                        
                        faceIndices.push_back(first + 1);
                        faceIndices.push_back(first + 5);
                        faceIndices.push_back(first + 6);
                        
                        faceIndices.push_back(first + 1);
                        faceIndices.push_back(first + 7);
                        faceIndices.push_back(first + 5);
                        
                        faceIndices.push_back(first + 0);
                        faceIndices.push_back(first + 3);
                        faceIndices.push_back(first + 2);
                        
                        faceIndices.push_back(first + 3);
                        faceIndices.push_back(first + 4);
                        faceIndices.push_back(first + 2);
                        
                        faceIndices.push_back(first + 4);
                        faceIndices.push_back(first + 5);
                        faceIndices.push_back(first + 2);
                        
                        faceIndices.push_back(first + 5);
                        faceIndices.push_back(first + 7);
                        faceIndices.push_back(first + 2);
                        
                        faceIndices.push_back(first + 0);
                        faceIndices.push_back(first + 2);
                        faceIndices.push_back(first + 1);
                        
                        faceIndices.push_back(first + 1);
                        faceIndices.push_back(first + 2);
                        faceIndices.push_back(first + 7);
                        
                        faceIndices.push_back(first + 3);
                        faceIndices.push_back(first + 1);
                        faceIndices.push_back(first + 5);
                        
                        faceIndices.push_back(first + 3);
                        faceIndices.push_back(first + 5);
                        faceIndices.push_back(first + 1);
                        
                        faceIndices.push_back(first + 1);
                        faceIndices.push_back(first + 5);
                        faceIndices.push_back(first + 2);
                        
                        faceIndices.push_back(first + 2);
                        faceIndices.push_back(first + 1);
                        faceIndices.push_back(first + 5);
                        
                        faceIndices.push_back(first + 3);
                        faceIndices.push_back(first + 1);
                        faceIndices.push_back(first + 2);
                        
                        faceIndices.push_back(first + 2);
                        faceIndices.push_back(first + 1);
                        faceIndices.push_back(first + 3);
                        
                        tetrahedronIndices.push_back(first + 0);
                        tetrahedronIndices.push_back(first + 1);
                        tetrahedronIndices.push_back(first + 2);
                        tetrahedronIndices.push_back(first + 3);
                        
                        tetrahedronIndices.push_back(first + 1);
                        tetrahedronIndices.push_back(first + 5);
                        tetrahedronIndices.push_back(first + 7);
                        tetrahedronIndices.push_back(first + 2);
                        
                        tetrahedronIndices.push_back(first + 1);
                        tetrahedronIndices.push_back(first + 5);
                        tetrahedronIndices.push_back(first + 6);
                        tetrahedronIndices.push_back(first + 3);
                        
                        tetrahedronIndices.push_back(first + 3);
                        tetrahedronIndices.push_back(first + 5);
                        tetrahedronIndices.push_back(first + 4);
                        tetrahedronIndices.push_back(first + 2);
                    }
                }
            }
            
            engine::util::vector<Vertex> vertices;
            engine::util::Set<Vector3f> deletedVectors;
            deletedVectors.set_empty_key(Vector3f(INFINITY, INFINITY, INFINITY));
            vertices.reserve(vectors.size() / 4);
            
            for(size_t i = 0; i < vectors.size(); ++i) {
                auto& vec = vectors[i];
                if(deletedVectors.find(vec) == deletedVectors.end()) {
                    auto newIndex = vertices.size();
                    
                    auto euclidianVector = cuboidToEuclidian * vec;
                    vertices.push_back(Vertex({euclidianVector[0], euclidianVector[1], euclidianVector[2]}, {.9, .9, .9}));
                    
                    for(size_t j = i; j < vectors.size(); ++j) {
                        if(vectors[j].isApprox(vec)) {
                            // Found the same vector => reduce and update indices
                            for(auto& ind : faceIndices) {
                                if(ind == j) {
                                    ind = newIndex;
                                }
                            }
                            for(auto& ind : tetrahedronIndices) {
                                if(ind == j) {
                                    ind = newIndex;
                                }
                            }
                        }
                    }
                    
                    deletedVectors.insert(vec);
                }
            }
            
            auto mesh = std::make_shared<Mesh>(vertices, faceIndices, DataUsagePattern::DYNAMIC_DRAW);
            
            return TetrahedronizedObject(mesh, tetrahedronIndices);
        }
    }
}