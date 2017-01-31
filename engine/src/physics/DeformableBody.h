#ifndef DEFORMABLEBODY_H
#define DEFORMABLEBODY_H

#include <eigen3/Eigen/Eigen>

#include "../renderer/Mesh.h"
#include "../IllegalArgumentException.h"

#include "Force.h"
#include "TetrahedronizedObject.h"

namespace engine {
    namespace physics {
        using namespace Eigen;
        using namespace engine::renderer;
        
        class DeformableBody {
            using SparseSolver = SparseLU<SparseMatrix<double, ColMajor>, COLAMDOrdering<SparseMatrix<double, ColMajor>::StorageIndex>>;
        protected:
            TetrahedronizedObject mesh;
            VectorXd restPosition;
            VectorXd currentPosition;
            
            double dampening; // in kg/s
            double youngsModulus; // in N/m² = Pa (Pascal)
            double poissonsRatio;
            
            SparseMatrix<double> stiffnessMatrix; // Called K in lecture
            SparseMatrix<double> dampeningMatrix; // Called C in lecture
            
            float stepSizeOnMatrixCalculation; // The last value of h.
            // If the current step size deviates more than
            // stepSizeDeviationPercentage % from the stepSizeOnMatrixCalculation
            // the stepMatrix is recalculated. Always updates if 0.
            float stepSizeDeviationPercentage;
            SparseMatrix<double, ColMajor> stepMatrix; // M + h² * K + h * C
            SparseSolver stepMatrixSolver;
            
            VectorXd lastVelocities;
            VectorXd vertexFreezer;
            
            SparseMatrix<double> calculateMaterialMatrix() const; // Called D in lecture
            SparseMatrix<double> calculateStiffnessMatrixForTetrahedron(size_t index) const; // Called K in lecture
            void combine3by3Block(SparseMatrix<double>& target, int targetRow, int targetCol, const SparseMatrix<double>& source, int sourceRow, int sourceCol) const;
            SparseMatrix<double> calculateStiffnessMatrix() const; // Called K in lecture
            SparseMatrix<double> calculateDampeningMatrix() const; // Called C in lecture
            SparseMatrix<double> calculateMassMatrix() const; // Called M in lecture
            SparseMatrix<double, ColMajor> calculateStepMatrix(float h) const; // M + h² * K + h * C
            void prepareStepMatrixSolver();
            
            VectorXd calculateCurrentDifferenceFromRestPosition() const;
            VectorXd calculateVelocities(float h, const VectorXf& forces) const;
            
            void updateStepMatrix(float h);
            void updateStepMatrixIfNecessary(float h);
            void calculateAndSetInitialState(float targetStepSize);
            
        public:
            DeformableBody(const TetrahedronizedObject& mesh, double dampening,
                    double youngsModulus, double poissonsRatio, float targetStepSize, float stepSizeDeviationPercentageForRecalculation = 2)
                    : mesh(mesh), currentPosition(this->mesh.getSimulationMesh().cast<double>()),
                        dampening(dampening), youngsModulus(youngsModulus),
                        poissonsRatio(poissonsRatio), stepSizeOnMatrixCalculation(0),
                        stepSizeDeviationPercentage(stepSizeDeviationPercentageForRecalculation) {
                if(this->poissonsRatio <= 0 || 0.5 <= this->poissonsRatio) {
                    throw IllegalArgumentException("The poisson's ratio must be between 0 and 0.5 (both exclusive), is %f.", this->poissonsRatio);
                }
                this->calculateAndSetInitialState(targetStepSize);
            }
            
            void step(float deltaT, const VectorXf& forces);
            void step(float deltaT, Force& force);
            
            void freezeVertex(size_t index);
            void freezeVertices(const engine::util::Array<size_t>& indices);
            void unfreezeVertex(size_t index);
            void unfreezeVertices(const engine::util::Array<size_t>& indices);
            
            const ObjectProperties& getProperties() const;
            
            VectorXd::Index getExpectedForceVectorSize() const;
            
            VectorXd& getCurrentPosition();
            const VectorXd& getCurrentPosition() const;
        };
    }
}

#endif /* DEFORMABLEBODY_H */

