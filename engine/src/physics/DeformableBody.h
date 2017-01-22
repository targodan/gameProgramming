#ifndef DEFORMABLEBODY_H
#define DEFORMABLEBODY_H

#include <eigen3/Eigen/Eigen>

#include "../renderer/Mesh.h"

#include "Force.h"

namespace engine {
    namespace physics {
        using namespace Eigen;
        using namespace engine::renderer;
        
        class DeformableBody {
            using SparseSolver = SparseLU<SparseMatrix<float, ColMajor>, COLAMDOrdering<SparseMatrix<float, ColMajor>::StorageIndex>>;
        protected:
            // The first 3 vertices must build the base plane.
            // The 4th vertex mustn't be in that same plane.
            Mesh& mesh;
            ObjectProperties properties;
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
            SparseMatrix<float, ColMajor> stepMatrix; // M + h² * K + h * C
            SparseSolver stepMatrixSolver;
            
            Matrix<float, 12, 1> lastVelocities;
            
            Matrix<float, 12, 1> calculatePlanarVectorsFromMesh() const;
            void setMeshFromPlanarVectors(const Matrix<float, 12, 1>& v);
            
            float calculateVolume() const;
            SparseMatrix<float> calculateMaterialMatrix() const; // Called D in lecture
            SparseMatrix<float> calculateStiffnessMatrix() const; // Called K in lecture
            SparseMatrix<float> calculateDampeningMatrix() const; // Called C in lecture
            SparseMatrix<float> calculateMassMatrix() const; // Called M in lecture
            SparseMatrix<float, ColMajor> calculateStepMatrix(float h) const; // M + h² * K + h * C
            void prepareStepMatrixSolver();
            
            Matrix<float, 12, 1> calculateCurrentDifferenceFromRestPosition() const;
            Matrix<float, 12, 1> calculateVelocities(float h, const Matrix<float, 12, 1>& forces) const;
            
            void updateStepMatrixIfNecessary(float h);
            void calculateAndSetInitialState(float targetStepSize);
            
        public:
            DeformableBody(Mesh& mesh, const ObjectProperties& properties, float mass, float dampening,
                    float youngsModulus, float poissonsRatio, float targetStepSize)
                    : mesh(mesh), properties(properties), mass(mass), dampening(dampening),
                        youngsModulus(youngsModulus), poissonsRatio(poissonsRatio) {
                this->calculateAndSetInitialState(targetStepSize);
            }
            
            void step(float deltaT, const Matrix<float, 12, 1>& forces);
            void step(float deltaT, Force& force);
        };
    }
}

#endif /* DEFORMABLEBODY_H */

