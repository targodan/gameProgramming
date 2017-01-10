#ifndef DEFORMABLEBODY_H
#define DEFORMABLEBODY_H

#include "../math/MatrixSq.h"
#include "../math/Vector.h"
#include "../renderer/Mesh.h"

namespace engine {
    namespace physics {
        using namespace engine::math;
        using namespace engine::renderer;
        
        class DeformableBody {
        protected:
            Mesh mesh;
            Vector<12> restPosition;
            Vector<12> currentPosition;
            
            float mass; // in kg
            float dampening; // in kg/s
            float youngsModulus; // in N/m²
            float poissonsRatio;
            
            MatrixSq<12> stiffnessMatrix; // Called K in lecture
            MatrixSq<12> dampeningMatrix; // Called C in lecture
            
            float stepSizeOnMatrixCalculation = 0; // The last value of h.
            // If the current step size deviates more than
            // stepSizeDeviationPercentage % from the stepSizeOnMatrixCalculation
            // the stepMatrix is recalculated. Always updates if 0.
            float stepSizeDeviationPercentage = 0.5;
            MatrixSq<12> stepMatrix; // inv(M + h² * K + h * C)
            
            Vector<12> lastVelocities;
            
            Vector<12> calculatePlanarVectorsFromMesh() const;
            void setMeshFromPlanarVectors(const Vector<12>& v);
            
            MatrixSq<6> calculateMaterialMatrix() const; // Called D in lecture
            MatrixSq<12> calculateStiffnessMatrix() const; // Called K in lecture
            MatrixSq<12> calculateDampeningMatrix() const; // Called C in lecture
            MatrixSq<12> calculateMassMatrix() const; // Called M in lecture
            MatrixSq<12> calculateStepMatrix(float h) const; // inv(M + h² * K + h * C)
            
            Vector<12> calculateCurrentDifferenceFromRestPosition() const;
            Vector<12> calculateVelocities(float h, Vector<12> forces) const;
            
            void updateStepMatrixIfNecessary(float h);
            
        public:
            DeformableBody(const Mesh& mesh, float mass, float dampening,
                    float youngsModulus, float poissonsRatio, float targetStepSize)
                    : mesh(mesh), mass(mass), dampening(dampening),
                        youngsModulus(youngsModulus), poissonsRatio(poissonsRatio) {
                this->restPosition = this->calculatePlanarVectorsFromMesh();
                this->currentPosition = this->restPosition;
                this->dampeningMatrix = this->calculateDampeningMatrix();
                this->stiffnessMatrix = this->calculateStiffnessMatrix();
            }
            
            void step(float h, Vector<12> forces);
        };
    }
}

#endif /* DEFORMABLEBODY_H */

