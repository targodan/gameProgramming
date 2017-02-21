#ifndef DEFORMABLEBODY_H
#define DEFORMABLEBODY_H

#include <eigen3/Eigen/Eigen>

#include "../renderer/Mesh.h"
#include "../IllegalArgumentException.h"
#include "../util/Set.h"

#include "Force.h"
#include "TetrahedronizedObject.h"

namespace std {
    template<>
    struct hash<pair<size_t,size_t>> {
       typedef pair<size_t,size_t> argument_type;
       typedef unsigned long long result_type;

       result_type operator() (const argument_type& x) const
       {
            // Yay, primes. :D
            return std::hash<result_type>()(static_cast<result_type>(x.first) * 3 + x.second * 5);
       }
    };
}

namespace engine {
    namespace physics {
        using namespace Eigen;
        using namespace engine::renderer;
        using engine::util::Set;
        
        class DeformableBody {
            using SparseSolver = SparseLU<SparseMatrix<double, ColMajor>, COLAMDOrdering<SparseMatrix<double, ColMajor>::StorageIndex>>;
        protected:
            TetrahedronizedObject mesh;
            VectorXf restPosition;
            
            double dampening; // in kg/s
            double youngsModulus; // in N/m² = Pa (Pascal)
            double poissonsRatio;
            
            double stressThresholdSqForBreaking;
            Set<size_t> deletedTetrahedra;
            Set<std::pair<size_t, size_t>> deletedEdges;
            
            SparseMatrix<double> stiffnessMatrix; // Called K in lecture
            SparseMatrix<double> dampeningMatrix; // Called C in lecture
            SparseMatrix<double> stressMatrix; // Calculates stress from deformation
            
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
            std::pair<SparseMatrix<double>, MatrixXd> calculateStiffnessAndStressMatrixForTetrahedron(size_t index) const; // Called K in lecture
            void combineBlock(int blockRows, int blockCols, SparseMatrix<double>& target, int targetRow, int targetCol, const SparseMatrix<double>& source, int sourceRow, int sourceCol) const;
            void combineBlock(int blockRows, int blockCols, SparseMatrix<double>& target, int targetRow, int targetCol, const MatrixXd& source, int sourceRow, int sourceCol) const;
            void combineStiffnessMatrices(SparseMatrix<double>& target, const SparseMatrix<double>& source, size_t tetraIndex) const;
            std::pair<SparseMatrix<double>, SparseMatrix<double>> calculateStiffnessAndStressMatrix() const; // Called K in lecture
            SparseMatrix<double> calculateDampeningMatrix() const; // Called C in lecture
            SparseMatrix<double> calculateMassMatrix() const; // Called M in lecture
            SparseMatrix<double, ColMajor> calculateStepMatrix(float h) const; // M + h² * K + h * C
            void prepareStepMatrixSolver();
            
            VectorXd calculateCurrentDifferenceFromRestPosition() const;
            VectorXd calculateVelocities(float h, const VectorXf& forces) const;
            VectorXd calculateStressPerTetrahedron(const VectorXd& deformation);
            
            Vector3f getVertexOfRestPosition(int index) const;
            MatrixXf getRestTetrahedron(int tetraIndex) const;

            void breakOnHighStress();
            std::pair<size_t, size_t> findBreakingEdgeOfTetrahedron(size_t tetraIndex) const;
            vector<size_t> findTetrahedraAdjacentToEdge(const std::pair<size_t, size_t>& edge) const;
            void deleteTetrahedronFromStiffnessAndStress(size_t tetraIndex);
            
            void updateStepMatrix(float h);
            void updateStepMatrixIfNecessary(float h);
            void calculateAndSetInitialState(float targetStepSize);
            
        public:
            DeformableBody(const TetrahedronizedObject& mesh, double dampening,
                    double youngsModulus, double poissonsRatio, float targetStepSize, double stressThresholdForBreaking = 0, float stepSizeDeviationPercentageForRecalculation = 2)
                    : mesh(mesh), dampening(dampening), youngsModulus(youngsModulus),
                        poissonsRatio(poissonsRatio), stressThresholdSqForBreaking(stressThresholdForBreaking),
                        stepSizeOnMatrixCalculation(0), stepSizeDeviationPercentage(stepSizeDeviationPercentageForRecalculation) {
                if(this->poissonsRatio <= 0 || 0.5 <= this->poissonsRatio) {
                    throw IllegalArgumentException("The poisson's ratio must be between 0 and 0.5 (both exclusive), is %f.", this->poissonsRatio);
                }
                this->deletedEdges.set_empty_key(std::make_pair(SIZE_MAX, SIZE_MAX));
                this->deletedTetrahedra.set_empty_key(SIZE_MAX);
                this->deletedTetrahedra.resize(this->mesh.getNumberOfTetrahedron());
                this->calculateAndSetInitialState(targetStepSize);
            }
            
            void step(float deltaT, const VectorXf& forces);
            void step(float deltaT, Force& force);
            
            void freezeVertex(size_t index);
            void freezeVertices(const engine::util::Array<size_t>& indices);
            void unfreezeVertex(size_t index);
            void unfreezeVertices(const engine::util::Array<size_t>& indices);
            void unfreezeAllVertices();
            
            void resetSimulation();
            
            bool isBreakingEnabled() const;
            
            const ObjectProperties& getProperties() const;
            
            VectorXd::Index getExpectedForceVectorSize() const;
            
            const VectorXf& getCurrentPosition() const;
        };
    }
}

#endif /* DEFORMABLEBODY_H */

