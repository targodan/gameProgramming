#include "Tetrahedronizer.h"
#include "../IllegalArgumentException.h"

#include "../util/vector.h"
#include "../renderer/Vertex.h"

#include "../util/Set.h"

#include "eigenHashes.h"

#include <easylogging++.h>

using engine::util::vector;
using namespace engine::renderer;

namespace engine {
    namespace physics {
        TetrahedronizedObject Tetrahedronizer::tetrahedronizeCuboid(
                    const Vector3f& corner,
                    const Vector3f& spanVectorOne, const Vector3f& spanVectorTwo, const Vector3f& spanVectorThree,
                    size_t numTetrahedronAlongSpanOne, size_t numTetrahedronAlongSpanTwo, size_t numTetrahedronAlongSpanThree,
                    float textureRepeatAlongSpanOne, float textureRepeatAlongSpanTwo, float textureRepeatAlongSpanThree,
                    float density
                ) {
            if(abs(spanVectorOne.cross(spanVectorTwo).dot(spanVectorThree)) < 1e-5) {
                throw IllegalArgumentException("The span vectors must not be in the same plane.");
            }
            
            MatrixXf cuboidToEuclidian(4, 4);
            cuboidToEuclidian <<
                    spanVectorOne, spanVectorTwo, spanVectorThree, corner,
                    Vector4f(0, 0, 0, 1).transpose();
            
            float offsetX = 1. / numTetrahedronAlongSpanOne;
            float offsetY = 1. / numTetrahedronAlongSpanTwo;
            float offsetZ = 1. / numTetrahedronAlongSpanThree;
            
            float texOffsetX = textureRepeatAlongSpanOne / numTetrahedronAlongSpanOne;
            float texOffsetY = textureRepeatAlongSpanTwo / numTetrahedronAlongSpanTwo;
            float texOffsetZ = textureRepeatAlongSpanThree / numTetrahedronAlongSpanThree;
            
            engine::util::vector<size_t> tetrahedronIndices;
            engine::util::vector<size_t> edgeIndices;
            engine::util::vector<Vector3f> allVectors;
            
            vector<size_t> tmpSurfaceVectorIndices;
            
            engine::util::vector<Vertex> innerVertices;
            engine::util::vector<GLuint> innerFaceIndices;
            engine::util::vector<Vertex> outerVertices;
            engine::util::vector<GLuint> outerFaceIndices;
            
            vector<vector<std::pair<size_t, size_t>>> tmpSimulationToRenderVertices;
            
            for(size_t xTetrahedron = 0; xTetrahedron < numTetrahedronAlongSpanOne; ++xTetrahedron) {
                for(size_t yTetrahedron = 0; yTetrahedron < numTetrahedronAlongSpanTwo; ++yTetrahedron) {
                    for(size_t zTetrahedron = 0; zTetrahedron < numTetrahedronAlongSpanThree; ++zTetrahedron) {
                        bool isX0 = xTetrahedron == 0;
                        bool isY0 = yTetrahedron == 0;
                        bool isZ0 = zTetrahedron == 0;
                        bool isX1 = xTetrahedron+1 == numTetrahedronAlongSpanOne;
                        bool isY1 = yTetrahedron+1 == numTetrahedronAlongSpanTwo;
                        bool isZ1 = zTetrahedron+1 == numTetrahedronAlongSpanThree;
                        
                        Vector4f corner_0_0_0 = cuboidToEuclidian * Vector4f(xTetrahedron * offsetX, yTetrahedron * offsetY, zTetrahedron * offsetZ, 1);
                        Vector4f corner_1_0_0 = cuboidToEuclidian * Vector4f(xTetrahedron * offsetX + offsetX, yTetrahedron * offsetY, zTetrahedron * offsetZ, 1);
                        Vector4f corner_1_0_1 = cuboidToEuclidian * Vector4f(xTetrahedron * offsetX + offsetX, yTetrahedron * offsetY, zTetrahedron * offsetZ + offsetZ, 1);
                        Vector4f corner_0_0_1 = cuboidToEuclidian * Vector4f(xTetrahedron * offsetX, yTetrahedron * offsetY, zTetrahedron * offsetZ + offsetZ, 1);

                        Vector4f corner_0_1_0 = cuboidToEuclidian * Vector4f(xTetrahedron * offsetX, yTetrahedron * offsetY + offsetY, zTetrahedron * offsetZ, 1);
                        Vector4f corner_1_1_0 = cuboidToEuclidian * Vector4f(xTetrahedron * offsetX + offsetX, yTetrahedron * offsetY + offsetY, zTetrahedron * offsetZ, 1);
                        Vector4f corner_1_1_1 = cuboidToEuclidian * Vector4f(xTetrahedron * offsetX + offsetX, yTetrahedron * offsetY + offsetY, zTetrahedron * offsetZ + offsetZ, 1);
                        Vector4f corner_0_1_1 = cuboidToEuclidian * Vector4f(xTetrahedron * offsetX, yTetrahedron * offsetY + offsetY, zTetrahedron * offsetZ + offsetZ, 1);
                        
                        vector<std::pair<size_t, size_t>> renderVerticesOf_0_0_0;
                        vector<std::pair<size_t, size_t>> renderVerticesOf_1_0_0;
                        vector<std::pair<size_t, size_t>> renderVerticesOf_1_0_1;
                        vector<std::pair<size_t, size_t>> renderVerticesOf_0_0_1;
                        vector<std::pair<size_t, size_t>> renderVerticesOf_0_1_0;
                        vector<std::pair<size_t, size_t>> renderVerticesOf_1_1_0;
                        vector<std::pair<size_t, size_t>> renderVerticesOf_1_1_1;
                        vector<std::pair<size_t, size_t>> renderVerticesOf_0_1_1;
                        
                        {   // All faces with x == 0
                            auto& veretexList = isX0 ? outerVertices : innerVertices;
                            auto& indexList = isX0 ? outerFaceIndices : innerFaceIndices;
                            auto meshIndex = isX0 ? 0 : 1;
                            Vector4f tmpNormal = cuboidToEuclidian * Vector4f(-1, 0, 0, 0);
                            auto normal = isX0 ? vec3(tmpNormal[0], tmpNormal[1], tmpNormal[2]) : vec3(1, 0, 0);

                            auto firstIndex = veretexList.size();
                            
                            veretexList.push_back(Vertex(vec4(corner_0_0_0), normal, vec2(zTetrahedron * texOffsetZ, yTetrahedron * texOffsetY))); // +0
                            veretexList.push_back(Vertex(vec4(corner_0_1_0), normal, vec2(zTetrahedron * texOffsetZ, yTetrahedron * texOffsetY + texOffsetY))); // +1
                            veretexList.push_back(Vertex(vec4(corner_0_1_1), normal, vec2(zTetrahedron * texOffsetZ + texOffsetZ, yTetrahedron * texOffsetY + texOffsetY))); // +2
                            veretexList.push_back(Vertex(vec4(corner_0_0_1), normal, vec2(zTetrahedron * texOffsetZ + texOffsetZ, yTetrahedron * texOffsetY))); // +3
                            
                            indexList.push_back(firstIndex + 0);
                            indexList.push_back(firstIndex + 2);
                            indexList.push_back(firstIndex + 1);
                            
                            indexList.push_back(firstIndex + 0);
                            indexList.push_back(firstIndex + 3);
                            indexList.push_back(firstIndex + 2);
                            
                            renderVerticesOf_0_0_0.push_back(std::make_pair(meshIndex, firstIndex + 0));
                            renderVerticesOf_0_1_0.push_back(std::make_pair(meshIndex, firstIndex + 1));
                            renderVerticesOf_0_1_1.push_back(std::make_pair(meshIndex, firstIndex + 2));
                            renderVerticesOf_0_0_1.push_back(std::make_pair(meshIndex, firstIndex + 3));
                        }
                        
                        {   // All faces with x == 1
                            auto& veretexList = isX1 ? outerVertices : innerVertices;
                            auto& indexList = isX1 ? outerFaceIndices : innerFaceIndices;
                            auto meshIndex = isX1 ? 0 : 1;
                            Vector4f tmpNormal = cuboidToEuclidian * Vector4f(1, 0, 0, 0);
                            auto normal = isX1 ? vec3(tmpNormal[0], tmpNormal[1], tmpNormal[2]) : vec3(1, 0, 0);

                            auto firstIndex = veretexList.size();
                            
                            veretexList.push_back(Vertex(vec4(corner_1_0_1), normal, vec2(textureRepeatAlongSpanThree - zTetrahedron * texOffsetZ, yTetrahedron * texOffsetY))); // +0
                            veretexList.push_back(Vertex(vec4(corner_1_0_0), normal, vec2(textureRepeatAlongSpanThree - zTetrahedron * texOffsetZ + texOffsetZ, yTetrahedron * texOffsetY))); // +1
                            veretexList.push_back(Vertex(vec4(corner_1_1_0), normal, vec2(textureRepeatAlongSpanThree - zTetrahedron * texOffsetZ + texOffsetZ, yTetrahedron * texOffsetY + texOffsetY))); // +2
                            veretexList.push_back(Vertex(vec4(corner_1_1_1), normal, vec2(textureRepeatAlongSpanThree - zTetrahedron * texOffsetZ, yTetrahedron * texOffsetY + texOffsetY))); // +3
                            
                            indexList.push_back(firstIndex + 0);
                            indexList.push_back(firstIndex + 2);
                            indexList.push_back(firstIndex + 3);
                            
                            indexList.push_back(firstIndex + 0);
                            indexList.push_back(firstIndex + 1);
                            indexList.push_back(firstIndex + 2);
                            
                            renderVerticesOf_1_0_1.push_back(std::make_pair(meshIndex, firstIndex + 0));
                            renderVerticesOf_1_0_0.push_back(std::make_pair(meshIndex, firstIndex + 1));
                            renderVerticesOf_1_1_0.push_back(std::make_pair(meshIndex, firstIndex + 2));
                            renderVerticesOf_1_1_1.push_back(std::make_pair(meshIndex, firstIndex + 3));
                        }
                        
                        {   // All faces with y == 0
                            auto& veretexList = isY0 ? outerVertices : innerVertices;
                            auto& indexList = isY0 ? outerFaceIndices : innerFaceIndices;
                            auto meshIndex = isY0 ? 0 : 1;
                            Vector4f tmpNormal = cuboidToEuclidian * Vector4f(0, -1, 0, 0);
                            auto normal = isY0 ? vec3(tmpNormal[0], tmpNormal[1], tmpNormal[2]) : vec3(1, 0, 0);

                            auto firstIndex = veretexList.size();
                            
                            veretexList.push_back(Vertex(vec4(corner_0_0_0), normal, vec2(xTetrahedron * texOffsetX, zTetrahedron * texOffsetZ))); // +0
                            veretexList.push_back(Vertex(vec4(corner_1_0_0), normal, vec2(xTetrahedron * texOffsetX + texOffsetX, zTetrahedron * texOffsetZ))); // +1
                            veretexList.push_back(Vertex(vec4(corner_1_0_1), normal, vec2(xTetrahedron * texOffsetX + texOffsetX, zTetrahedron * texOffsetZ + texOffsetZ))); // +2
                            veretexList.push_back(Vertex(vec4(corner_0_0_1), normal, vec2(xTetrahedron * texOffsetX, zTetrahedron * texOffsetZ + texOffsetZ))); // +3
                            
                            indexList.push_back(firstIndex + 0);
                            indexList.push_back(firstIndex + 2);
                            indexList.push_back(firstIndex + 3);
                            
                            indexList.push_back(firstIndex + 0);
                            indexList.push_back(firstIndex + 1);
                            indexList.push_back(firstIndex + 2);
                            
                            renderVerticesOf_0_0_0.push_back(std::make_pair(meshIndex, firstIndex + 0));
                            renderVerticesOf_1_0_0.push_back(std::make_pair(meshIndex, firstIndex + 1));
                            renderVerticesOf_1_0_1.push_back(std::make_pair(meshIndex, firstIndex + 2));
                            renderVerticesOf_0_0_1.push_back(std::make_pair(meshIndex, firstIndex + 3));
                        }
                        
                        {   // All faces with y == 1
                            auto& veretexList = isY1 ? outerVertices : innerVertices;
                            auto& indexList = isY1 ? outerFaceIndices : innerFaceIndices;
                            auto meshIndex = isY1 ? 0 : 1;
                            Vector4f tmpNormal = cuboidToEuclidian * Vector4f(0, 1, 0, 0);
                            auto normal = isY1 ? vec3(tmpNormal[0], tmpNormal[1], tmpNormal[2]) : vec3(1, 0, 0);

                            auto firstIndex = veretexList.size();
                            
                            veretexList.push_back(Vertex(vec4(corner_0_1_0), normal, vec2(zTetrahedron * texOffsetZ, xTetrahedron * texOffsetX))); // +0
                            veretexList.push_back(Vertex(vec4(corner_1_1_0), normal, vec2(zTetrahedron * texOffsetZ, xTetrahedron * texOffsetX + texOffsetX))); // +1
                            veretexList.push_back(Vertex(vec4(corner_1_1_1), normal, vec2(zTetrahedron * texOffsetZ + texOffsetZ, xTetrahedron * texOffsetX + texOffsetX))); // +2
                            veretexList.push_back(Vertex(vec4(corner_0_1_1), normal, vec2(zTetrahedron * texOffsetZ + texOffsetZ, xTetrahedron * texOffsetX))); // +3
                            
                            indexList.push_back(firstIndex + 0);
                            indexList.push_back(firstIndex + 3);
                            indexList.push_back(firstIndex + 1);
                            
                            indexList.push_back(firstIndex + 2);
                            indexList.push_back(firstIndex + 1);
                            indexList.push_back(firstIndex + 3);
                            
                            renderVerticesOf_0_1_0.push_back(std::make_pair(meshIndex, firstIndex + 0));
                            renderVerticesOf_1_1_0.push_back(std::make_pair(meshIndex, firstIndex + 1));
                            renderVerticesOf_1_1_1.push_back(std::make_pair(meshIndex, firstIndex + 2));
                            renderVerticesOf_0_1_1.push_back(std::make_pair(meshIndex, firstIndex + 3));
                        }
                        
                        {   // All faces with z == 0
                            auto& veretexList = isZ0 ? outerVertices : innerVertices;
                            auto& indexList = isZ0 ? outerFaceIndices : innerFaceIndices;
                            auto meshIndex = isZ0 ? 0 : 1;
                            Vector4f tmpNormal = cuboidToEuclidian * Vector4f(0, 0, -1, 0);
                            auto normal = isZ0 ? vec3(tmpNormal[0], tmpNormal[1], tmpNormal[2]) : vec3(1, 0, 0);

                            auto firstIndex = veretexList.size();
                            
                            veretexList.push_back(Vertex(vec4(corner_1_0_0), normal, vec2(textureRepeatAlongSpanOne - xTetrahedron * texOffsetX, yTetrahedron * texOffsetY))); // +1
                            veretexList.push_back(Vertex(vec4(corner_0_0_0), normal, vec2(textureRepeatAlongSpanOne - xTetrahedron * texOffsetX + texOffsetX, yTetrahedron * texOffsetY))); // +0
                            veretexList.push_back(Vertex(vec4(corner_1_1_0), normal, vec2(textureRepeatAlongSpanOne - xTetrahedron * texOffsetX, yTetrahedron * texOffsetY + texOffsetY))); // +2
                            veretexList.push_back(Vertex(vec4(corner_0_1_0), normal, vec2(textureRepeatAlongSpanOne - xTetrahedron * texOffsetX + texOffsetX, yTetrahedron * texOffsetY + texOffsetY))); // +3
                            
                            indexList.push_back(firstIndex + 0);
                            indexList.push_back(firstIndex + 1);
                            indexList.push_back(firstIndex + 2);
                            
                            indexList.push_back(firstIndex + 1);
                            indexList.push_back(firstIndex + 3);
                            indexList.push_back(firstIndex + 2);
                            
                            renderVerticesOf_1_0_0.push_back(std::make_pair(meshIndex, firstIndex + 0));
                            renderVerticesOf_0_0_0.push_back(std::make_pair(meshIndex, firstIndex + 1));
                            renderVerticesOf_1_1_0.push_back(std::make_pair(meshIndex, firstIndex + 2));
                            renderVerticesOf_0_1_0.push_back(std::make_pair(meshIndex, firstIndex + 3));
                        }
                        
                        {   // All faces with z == 1
                            auto& veretexList = isZ1 ? outerVertices : innerVertices;
                            auto& indexList = isZ1 ? outerFaceIndices : innerFaceIndices;
                            auto meshIndex = isZ1 ? 0 : 1;
                            Vector4f tmpNormal = cuboidToEuclidian * Vector4f(0, 0, 1, 0);
                            auto normal = isZ1 ? vec3(tmpNormal[0], tmpNormal[1], tmpNormal[2]) : vec3(1, 0, 0);

                            auto firstIndex = veretexList.size();
                            
                            veretexList.push_back(Vertex(vec4(corner_0_0_1), normal, vec2(xTetrahedron * texOffsetX, yTetrahedron * texOffsetY))); // +0
                            veretexList.push_back(Vertex(vec4(corner_1_0_1), normal, vec2(xTetrahedron * texOffsetX + texOffsetX, yTetrahedron * texOffsetY))); // +1
                            veretexList.push_back(Vertex(vec4(corner_1_1_1), normal, vec2(xTetrahedron * texOffsetX + texOffsetX, yTetrahedron * texOffsetY + texOffsetY))); // +2
                            veretexList.push_back(Vertex(vec4(corner_0_1_1), normal, vec2(xTetrahedron * texOffsetX, yTetrahedron * texOffsetY + texOffsetY))); // +3
                            
                            indexList.push_back(firstIndex + 0);
                            indexList.push_back(firstIndex + 1);
                            indexList.push_back(firstIndex + 3);
                            
                            indexList.push_back(firstIndex + 1);
                            indexList.push_back(firstIndex + 2);
                            indexList.push_back(firstIndex + 3);
                            
                            renderVerticesOf_0_0_1.push_back(std::make_pair(meshIndex, firstIndex + 0));
                            renderVerticesOf_1_0_1.push_back(std::make_pair(meshIndex, firstIndex + 1));
                            renderVerticesOf_1_1_1.push_back(std::make_pair(meshIndex, firstIndex + 2));
                            renderVerticesOf_0_1_1.push_back(std::make_pair(meshIndex, firstIndex + 3));
                        }
                        
                        // All purely internal faces
                        auto firstIndex = innerVertices.size();
                        
                        float uvHeight = sqrtf(3) / 3;
                            
                        innerVertices.push_back(Vertex(vec4(corner_0_1_1), {1, 0, 0}, {0,    0})); // +0
                        innerVertices.push_back(Vertex(vec4(corner_1_0_1), {1, 0, 0}, {1./3, uvHeight})); // +1
                        innerVertices.push_back(Vertex(vec4(corner_1_1_0), {1, 0, 0}, {1,    uvHeight})); // +2
                        innerVertices.push_back(Vertex(vec4(corner_0_0_0), {1, 0, 0}, {2./3, 0})); // +3

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
                        
                        renderVerticesOf_0_1_1.push_back(std::make_pair(1, firstIndex + 0));
                        renderVerticesOf_1_0_1.push_back(std::make_pair(1, firstIndex + 1));
                        renderVerticesOf_1_1_0.push_back(std::make_pair(1, firstIndex + 2));
                        renderVerticesOf_0_0_0.push_back(std::make_pair(1, firstIndex + 3));
                        
                        // Compose simulation stuff
                        
                        tmpSimulationToRenderVertices.push_back(renderVerticesOf_0_0_0);
                        tmpSimulationToRenderVertices.push_back(renderVerticesOf_1_0_0);
                        tmpSimulationToRenderVertices.push_back(renderVerticesOf_1_0_1);
                        tmpSimulationToRenderVertices.push_back(renderVerticesOf_0_0_1);
                        tmpSimulationToRenderVertices.push_back(renderVerticesOf_0_1_0);
                        tmpSimulationToRenderVertices.push_back(renderVerticesOf_1_1_0);
                        tmpSimulationToRenderVertices.push_back(renderVerticesOf_1_1_1);
                        tmpSimulationToRenderVertices.push_back(renderVerticesOf_0_1_1);
                        
                        auto firstSimulationIndex = allVectors.size();
                        allVectors.push_back(corner_0_0_0.segment<3>(0)); // + 0
                        allVectors.push_back(corner_1_0_0.segment<3>(0)); // + 1
                        allVectors.push_back(corner_1_0_1.segment<3>(0)); // + 2
                        allVectors.push_back(corner_0_0_1.segment<3>(0)); // + 3
                        allVectors.push_back(corner_0_1_0.segment<3>(0)); // + 4
                        allVectors.push_back(corner_1_1_0.segment<3>(0)); // + 5
                        allVectors.push_back(corner_1_1_1.segment<3>(0)); // + 6
                        allVectors.push_back(corner_0_1_1.segment<3>(0)); // + 7
                        
                        tetrahedronIndices.push_back(firstSimulationIndex + 0);
                        tetrahedronIndices.push_back(firstSimulationIndex + 3);
                        tetrahedronIndices.push_back(firstSimulationIndex + 2);
                        tetrahedronIndices.push_back(firstSimulationIndex + 7);
                        
                        tetrahedronIndices.push_back(firstSimulationIndex + 0);
                        tetrahedronIndices.push_back(firstSimulationIndex + 7);
                        tetrahedronIndices.push_back(firstSimulationIndex + 5);
                        tetrahedronIndices.push_back(firstSimulationIndex + 4);
                        
                        tetrahedronIndices.push_back(firstSimulationIndex + 7);
                        tetrahedronIndices.push_back(firstSimulationIndex + 2);
                        tetrahedronIndices.push_back(firstSimulationIndex + 5);
                        tetrahedronIndices.push_back(firstSimulationIndex + 6);
                        
                        tetrahedronIndices.push_back(firstSimulationIndex + 0);
                        tetrahedronIndices.push_back(firstSimulationIndex + 5);
                        tetrahedronIndices.push_back(firstSimulationIndex + 2);
                        tetrahedronIndices.push_back(firstSimulationIndex + 1);
                        
                        tetrahedronIndices.push_back(firstSimulationIndex + 0);
                        tetrahedronIndices.push_back(firstSimulationIndex + 7);
                        tetrahedronIndices.push_back(firstSimulationIndex + 2);
                        tetrahedronIndices.push_back(firstSimulationIndex + 5);
                        
                        if(isX0) {
                            tmpSurfaceVectorIndices.push_back(firstSimulationIndex + 0);
                            tmpSurfaceVectorIndices.push_back(firstSimulationIndex + 3);
                            tmpSurfaceVectorIndices.push_back(firstSimulationIndex + 4);
                            tmpSurfaceVectorIndices.push_back(firstSimulationIndex + 7);
                        }
                        if(isX1) {
                            tmpSurfaceVectorIndices.push_back(firstSimulationIndex + 1);
                            tmpSurfaceVectorIndices.push_back(firstSimulationIndex + 2);
                            tmpSurfaceVectorIndices.push_back(firstSimulationIndex + 5);
                            tmpSurfaceVectorIndices.push_back(firstSimulationIndex + 6);
                        }
                        if(isY0) {
                            tmpSurfaceVectorIndices.push_back(firstSimulationIndex + 0);
                            tmpSurfaceVectorIndices.push_back(firstSimulationIndex + 1);
                            tmpSurfaceVectorIndices.push_back(firstSimulationIndex + 2);
                            tmpSurfaceVectorIndices.push_back(firstSimulationIndex + 3);
                        }
                        if(isY1) {
                            tmpSurfaceVectorIndices.push_back(firstSimulationIndex + 4);
                            tmpSurfaceVectorIndices.push_back(firstSimulationIndex + 5);
                            tmpSurfaceVectorIndices.push_back(firstSimulationIndex + 6);
                            tmpSurfaceVectorIndices.push_back(firstSimulationIndex + 7);
                        }
                        if(isZ0) {
                            tmpSurfaceVectorIndices.push_back(firstSimulationIndex + 0);
                            tmpSurfaceVectorIndices.push_back(firstSimulationIndex + 1);
                            tmpSurfaceVectorIndices.push_back(firstSimulationIndex + 4);
                            tmpSurfaceVectorIndices.push_back(firstSimulationIndex + 5);
                        }
                        if(isZ1) {
                            tmpSurfaceVectorIndices.push_back(firstSimulationIndex + 2);
                            tmpSurfaceVectorIndices.push_back(firstSimulationIndex + 3);
                            tmpSurfaceVectorIndices.push_back(firstSimulationIndex + 6);
                            tmpSurfaceVectorIndices.push_back(firstSimulationIndex + 7);
                        }
                        
                        // Edges
                        if(isX0 && isY0) {
                            edgeIndices.push_back(firstSimulationIndex + 0);
                            edgeIndices.push_back(firstSimulationIndex + 3);
                        }
                        if(isX0 && isY1) {
                            edgeIndices.push_back(firstSimulationIndex + 4);
                            edgeIndices.push_back(firstSimulationIndex + 7);
                        }
                        if(isX0 && isZ0) {
                            edgeIndices.push_back(firstSimulationIndex + 0);
                            edgeIndices.push_back(firstSimulationIndex + 4);
                        }
                        if(isX0 && isZ1) {
                            edgeIndices.push_back(firstSimulationIndex + 3);
                            edgeIndices.push_back(firstSimulationIndex + 7);
                        }
                        if(isX1 && isY0) {
                            edgeIndices.push_back(firstSimulationIndex + 1);
                            edgeIndices.push_back(firstSimulationIndex + 2);
                        }
                        if(isX1 && isY1) {
                            edgeIndices.push_back(firstSimulationIndex + 5);
                            edgeIndices.push_back(firstSimulationIndex + 6);
                        }
                        if(isX1 && isZ0) {
                            edgeIndices.push_back(firstSimulationIndex + 1);
                            edgeIndices.push_back(firstSimulationIndex + 5);
                        }
                        if(isX1 && isZ1) {
                            edgeIndices.push_back(firstSimulationIndex + 2);
                            edgeIndices.push_back(firstSimulationIndex + 6);
                        }
                        if(isY0 && isZ0) {
                            edgeIndices.push_back(firstSimulationIndex + 0);
                            edgeIndices.push_back(firstSimulationIndex + 1);
                        }
                        if(isY0 && isZ1) {
                            edgeIndices.push_back(firstSimulationIndex + 2);
                            edgeIndices.push_back(firstSimulationIndex + 3);
                        }
                        if(isY1 && isZ0) {
                            edgeIndices.push_back(firstSimulationIndex + 4);
                            edgeIndices.push_back(firstSimulationIndex + 5);
                        }
                        if(isY1 && isZ1) {
                            edgeIndices.push_back(firstSimulationIndex + 6);
                            edgeIndices.push_back(firstSimulationIndex + 7);
                        }
                    }
                }
            }
            
            engine::util::vector<Vector3f> simulationVectors;
            vector<Array<std::pair<size_t, size_t>>> simulationToRenderVertices;
            vector<size_t> surfaceVectorIndices;
            engine::util::Set<size_t> deletedVectors;
            deletedVectors.set_empty_key(SIZE_MAX);
            
            for(size_t i = 0; i < allVectors.size(); ++i) {
                auto& vec = allVectors[i];
                if(deletedVectors.find(i) == deletedVectors.end()) {
                    auto newIndex = simulationVectors.size();
                    
                    vector<std::pair<size_t, size_t>> simToRenderThisVertex;
                    simToRenderThisVertex.insert(simToRenderThisVertex.end(), tmpSimulationToRenderVertices[i].begin(), tmpSimulationToRenderVertices[i].end());
                    
                    for(size_t j = i; j < allVectors.size(); ++j) {
                        if(allVectors[j].isApprox(vec)) {
                            // Found the same vector => reduce and update indices
                            for(auto& ind : tetrahedronIndices) {
                                if(ind == j) {
                                    ind = newIndex;
                                }
                            }
                            for(auto& ind : edgeIndices) {
                                if(ind == j) {
                                    ind = newIndex;
                                }
                            }
                            simToRenderThisVertex.insert(simToRenderThisVertex.end(), tmpSimulationToRenderVertices[j].begin(), tmpSimulationToRenderVertices[j].end());
                    
                            deletedVectors.insert(j);
                        }
                    }
                    bool isSurface = false;
                    for(size_t j = i; j < allVectors.size(); ++j) {
                        if(allVectors[j].isApprox(vec)) {
                            for(auto ind : tmpSurfaceVectorIndices) {
                                if(ind == j) {
                                    isSurface = true;
                                    break;
                                }
                            }
                        }
                        if(isSurface) {
                            surfaceVectorIndices.push_back(newIndex);
                            break;
                        }
                    }
                    
                    simulationVectors.push_back(vec);
                    simulationToRenderVertices.push_back(simToRenderThisVertex);
                }
            }
            
            VectorXf simulationMesh(simulationVectors.size() * 3);
            int i = 0;
            for(auto& v : simulationVectors) {
                simulationMesh[i++] = v[0];
                simulationMesh[i++] = v[1];
                simulationMesh[i++] = v[2];
            }
            
            vector<std::shared_ptr<Mesh>> renderMeshes;
            renderMeshes.reserve(2);
            renderMeshes.push_back(std::make_shared<Mesh>(outerVertices, outerFaceIndices, DataUsagePattern::STREAM_DRAW));
            renderMeshes.push_back(std::make_shared<Mesh>(innerVertices, innerFaceIndices, DataUsagePattern::STREAM_DRAW));
             
            return TetrahedronizedObject(simulationMesh, surfaceVectorIndices, density, renderMeshes, simulationToRenderVertices, tetrahedronIndices, edgeIndices);
        }
    }
}