#ifndef DEFORMABLEBODY_H
#define DEFORMABLEBODY_H

#include <eigen3/Eigen/Eigen>

#include "../renderer/Mesh.h"

namespace engine {
    namespace physics {
        using namespace Eigen;
        using namespace engine::renderer;
        
        class DeformableBody {
        protected:
            Mesh mesh;
            Matrix<float, 12, 1> restPosition;
            Matrix<float, 12, 1> currentPosition;
            
            float mass; // in kg
            float dampening; // in kg/s
            float youngsModulus; // in N/m² = Pa (Pascal)
            float poissonsRatio;
            
            SparseMatrix<float> stiffnessMatrix; // Called K in lecture
            SparseMatrix<float> dampeningMatrix; // Called C in lecture
            
            float stepSizeOnMatrixCalculation = 0; // The last value of h.
            // If the current step size deviates more than
            // stepSizeDeviationPercentage % from the stepSizeOnMatrixCalculation
            // the stepMatrix is recalculated. Always updates if 0.
            float stepSizeDeviationPercentage = 0.5;
            HouseholderQR<Matrix<float, 12, 12>> stepMatrix; // inv(M + h² * K + h * C)
            
            Matrix<float, 12, 1> lastVelocities;
            
            Matrix<float, 12, 1> calculatePlanarVectorsFromMesh() const;
            void setMeshFromPlanarVectors(const Matrix<float, 12, 1>& v);
            
            SparseMatrix<float> calculateMaterialMatrix() const; // Called D in lecture
            SparseMatrix<float> calculateStiffnessMatrix() const; // Called K in lecture
            SparseMatrix<float> calculateDampeningMatrix() const; // Called C in lecture
            SparseMatrix<float> calculateMassMatrix() const; // Called M in lecture
            HouseholderQR<Matrix<float, 12, 12>> calculateStepMatrix(float h) const; // inv(M + h² * K + h * C)
            
            Matrix<float, 12, 1> calculateCurrentDifferenceFromRestPosition() const;
            Matrix<float, 12, 1> calculateVelocities(float h, Matrix<float, 12, 1> forces) const;
            
            void updateStepMatrixIfNecessary(float h);
            void calculateAndSetInitialState(float targetStepSize);
            
        public:
            DeformableBody(const Mesh& mesh, float mass, float dampening,
                    float youngsModulus, float poissonsRatio, float targetStepSize)
                    : mesh(mesh), mass(mass), dampening(dampening),
                        youngsModulus(youngsModulus), poissonsRatio(poissonsRatio) {
                this->calculateAndSetInitialState(targetStepSize);
            }
            
            void step(float h, Matrix<float, 12, 1> forces);
        };
    }
}

#endif /* DEFORMABLEBODY_H */

