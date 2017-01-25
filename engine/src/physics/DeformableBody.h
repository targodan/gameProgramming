#ifndef DEFORMABLEBODY_H
#define DEFORMABLEBODY_H

#include <eigen3/Eigen/Eigen>

#include "../renderer/Mesh.h"

#include "Force.h"
#include "TetrahedronizedMesh.h"

namespace engine {
    namespace physics {
        using namespace Eigen;
        using namespace engine::renderer;
        
        class DeformableBody {
            using SparseSolver = SparseLU<SparseMatrix<float, ColMajor>, COLAMDOrdering<SparseMatrix<float, ColMajor>::StorageIndex>>;
        protected:
            TetrahedronizedMesh mesh;
            ObjectProperties properties;
            VectorXf restPosition;
            VectorXf& currentPosition;
            
            float mass; // in kg
            float dampening; // in kg/s
            float youngsModulus; // in N/m² = Pa (Pascal)
            float poissonsRatio;
            
            SparseMatrix<float> stiffnessMatrix; // Called K in lecture
            SparseMatrix<float> dampeningMatrix; // Called C in lecture
            
            float stepSizeOnMatrixCalculation; // The last value of h.
            // If the current step size deviates more than
            // stepSizeDeviationPercentage % from the stepSizeOnMatrixCalculation
            // the stepMatrix is recalculated. Always updates if 0.
            float stepSizeDeviationPercentage;
            SparseMatrix<float, ColMajor> stepMatrix; // M + h² * K + h * C
            SparseSolver stepMatrixSolver;
            
            VectorXf lastVelocities;
            
            SparseMatrix<float> calculateMaterialMatrix() const; // Called D in lecture
            SparseMatrix<float> calculateStiffnessMatrix() const; // Called K in lecture
            SparseMatrix<float> calculateDampeningMatrix() const; // Called C in lecture
            SparseMatrix<float> calculateMassMatrix() const; // Called M in lecture
            SparseMatrix<float, ColMajor> calculateStepMatrix(float h) const; // M + h² * K + h * C
            void prepareStepMatrixSolver();
            
            VectorXf calculateCurrentDifferenceFromRestPosition() const;
            VectorXf calculateVelocities(float h, const VectorXf& forces) const;
            
            void updateStepMatrix(float h);
            void updateStepMatrixIfNecessary(float h);
            void calculateAndSetInitialState(float targetStepSize);
            
        public:
            DeformableBody(const TetrahedronizedMesh& mesh, const ObjectProperties& properties, float mass, float dampening,
                    float youngsModulus, float poissonsRatio, float targetStepSize, float stepSizeDeviationPercentageForRecalculation = 2)
                    : mesh(mesh), properties(properties), currentPosition(this->properties.allVertices),
                        mass(mass), dampening(dampening), youngsModulus(youngsModulus),
                        poissonsRatio(poissonsRatio), stepSizeOnMatrixCalculation(0),
                        stepSizeDeviationPercentage(stepSizeDeviationPercentageForRecalculation) {
                this->calculateAndSetInitialState(targetStepSize);
            }
            
            void step(float deltaT, const VectorXf& forces);
            void step(float deltaT, Force& force);
        };
    }
}

#endif /* DEFORMABLEBODY_H */

