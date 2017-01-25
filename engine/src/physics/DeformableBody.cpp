#include "DeformableBody.h"

#include <easylogging++.h>

#ifndef ABS
#define ABS(x) ((x) < 0 ? -(x) : (x))
#endif

namespace engine {
    namespace physics {
        SparseMatrix<float> DeformableBody::calculateMaterialMatrix() const {
            SparseMatrix<float> materialMat(6, 6);
            materialMat.reserve(12);
            materialMat.insert(0, 0) = 1 - this->poissonsRatio;
            materialMat.insert(0, 1) = this->poissonsRatio;
            materialMat.insert(0, 2) = this->poissonsRatio;
            materialMat.insert(1, 0) = this->poissonsRatio;
            materialMat.insert(1, 1) = 1 - this->poissonsRatio;
            materialMat.insert(1, 2) = this->poissonsRatio;
            materialMat.insert(2, 0) = this->poissonsRatio;
            materialMat.insert(2, 1) = this->poissonsRatio;
            materialMat.insert(2, 2) = 1 - this->poissonsRatio;
            materialMat.insert(3, 3) = 1 - 2*this->poissonsRatio;
            materialMat.insert(4, 4) = 1 - 2*this->poissonsRatio;
            materialMat.insert(5, 5) = 1 - 2*this->poissonsRatio;
            return (this->youngsModulus / (1+this->poissonsRatio)*(1-2*this->poissonsRatio))
                    * materialMat;
        }
        
        SparseMatrix<float> DeformableBody::calculateStiffnessMatrix() const {
            TIMED_FUNC(timerCalcStiffnessMat);
            
            auto numVertices = this->properties.allVertices.rows() / 3;
            
            const Eigen::Map<MatrixXf> vertices(const_cast<float*>(this->properties.allVertices.data()), 3, numVertices);
            MatrixXf A_inv(numVertices, numVertices);
            A_inv << vertices, MatrixXf::Ones(numVertices - 3, numVertices);
            
            FullPivLU<MatrixXf> lu(A_inv);
            auto A = lu.inverse();
            
            auto B = SparseMatrix<float>(6, this->properties.allVertices.rows());
            B.reserve(9 * numVertices);
            
            for(int i = 0; i < numVertices; ++i) {
                auto basInd = i * 3;
                B.insert(0, basInd + 0) = A(i, 0);
                B.insert(1, basInd + 1) = A(i, 1);
                B.insert(2, basInd + 2) = A(i, 2);
                
                B.insert(3, basInd + 0) = A(i, 1);
                B.insert(3, basInd + 1) = A(i, 0);
                B.insert(4, basInd + 0) = A(i, 2);
                B.insert(4, basInd + 2) = A(i, 0);
                B.insert(5, basInd + 1) = A(i, 2);
                B.insert(5, basInd + 2) = A(i, 1);
            }
            
            return this->mesh.calculateVolume() * B.transpose() * this->calculateMaterialMatrix() * B;
        }
        
        SparseMatrix<float> DeformableBody::calculateDampeningMatrix() const {
            SparseMatrix<float> dampening(this->properties.allVertices.rows(), this->properties.allVertices.rows());
            dampening.setIdentity();
            return this->dampening * dampening;
        }
        
        SparseMatrix<float> DeformableBody::calculateMassMatrix() const {
            SparseMatrix<float> mass(this->properties.allVertices.rows(), this->properties.allVertices.rows());
            mass.setIdentity();
            return this->mass * mass;
        }
        
        SparseMatrix<float, ColMajor> DeformableBody::calculateStepMatrix(float h) const {
            SparseMatrix<float, ColMajor> mat = this->calculateMassMatrix() + h * (h * this->stiffnessMatrix + this->dampeningMatrix);
            mat.makeCompressed();
            return mat;
        }
        
        void DeformableBody::prepareStepMatrixSolver() {
            this->stepMatrixSolver.analyzePattern(this->stepMatrix);
            this->stepMatrixSolver.factorize(this->stepMatrix);
        }
            
        void DeformableBody::updateStepMatrix(float h) {
            this->stepMatrix = this->calculateStepMatrix(h);
            this->prepareStepMatrixSolver();
            this->stepSizeOnMatrixCalculation = h;
        }
            
        void DeformableBody::updateStepMatrixIfNecessary(float h) {
            float deviation = ABS(this->stepSizeOnMatrixCalculation - h) / (float)this->stepSizeOnMatrixCalculation;
            if(this->stepSizeOnMatrixCalculation == 0
                    || deviation >= this->stepSizeDeviationPercentage) {
                if(this->stepSizeOnMatrixCalculation != 0) {
                    LOG(WARNING) << "Recalculating step matrix. Time delta deviation: " << deviation << " %";
                }
                this->updateStepMatrix(h);
            }
        }
        
        VectorXf DeformableBody::calculateCurrentDifferenceFromRestPosition() const {
            return this->currentPosition - this->restPosition;
        }
        
        VectorXf DeformableBody::calculateVelocities(float h, const VectorXf& forces) const {
            return this->lastVelocities +
                    (
                        this->stepMatrixSolver.solve(
                            h * (
                                forces
                                - this->dampeningMatrix * this->lastVelocities
                                - this->stiffnessMatrix * (
                                    this->calculateCurrentDifferenceFromRestPosition()
                                    - h * this->lastVelocities
                                )
                            )
                        )
                    );
        }
        
        void DeformableBody::calculateAndSetInitialState(float targetStepSize) {
            // currentPosition points to the objectProperties->allVertices
            this->restPosition = this->currentPosition;
            this->lastVelocities = VectorXf::Zero(this->currentPosition.rows());
            this->dampeningMatrix = this->calculateDampeningMatrix();
            this->stiffnessMatrix = this->calculateStiffnessMatrix();
            this->updateStepMatrix(targetStepSize);
        }
        
        void DeformableBody::step(float deltaT, Force& force) {
            auto forces = force.getForceOnVertices(this->properties);
            if(forces.rows() > 0) {
                this->step(deltaT, forces);
            }
        }
        
        void DeformableBody::step(float deltaT, const VectorXf& forces) {
            this->updateStepMatrixIfNecessary(deltaT);
            this->lastVelocities = this->calculateVelocities(deltaT, forces);
            this->currentPosition += deltaT * this->lastVelocities;
            this->mesh.updateMeshFromPlanarVector(this->currentPosition);
        }
    }
}