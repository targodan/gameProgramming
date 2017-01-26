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
        
        SparseMatrix<float> DeformableBody::calculateStiffnessMatrixForTetrahedron(size_t index) const {
            MatrixXf A_inv(4, 4);
            A_inv << this->mesh.getTetrahedron(index), MatrixXf::Ones(1, 4);
            
            auto A = A_inv.inverse();
            
            auto B = SparseMatrix<float>(6, 12);
            B.reserve(36);
            
            for(int i = 0; i < 4; ++i) {
                auto baseInd = i * 3;
                B.insert(0, baseInd + 0) = A(i, 0);
                B.insert(1, baseInd + 1) = A(i, 1);
                B.insert(2, baseInd + 2) = A(i, 2);
                
                B.insert(3, baseInd + 0) = A(i, 1);
                B.insert(3, baseInd + 1) = A(i, 0);
                B.insert(4, baseInd + 1) = A(i, 2);
                B.insert(4, baseInd + 2) = A(i, 1);
                B.insert(5, baseInd + 0) = A(i, 2);
                B.insert(5, baseInd + 2) = A(i, 0);
            }
            
            return this->mesh.calculateVolumeOfTetrahedron(index) * B.transpose() * this->calculateMaterialMatrix() * B;
        }
        
        SparseMatrix<float> DeformableBody::calculateStiffnessMatrix() const {
            SparseMatrix<float> stiffnessMatrix(this->properties.allVertices.rows(), this->properties.allVertices.rows());
            stiffnessMatrix.reserve(12 * 12 + 63 * this->mesh.getNumberOfTetrahedron());
            
            for(size_t tetraIndex = 0; tetraIndex < this->mesh.getNumberOfTetrahedron(); ++tetraIndex) {
                auto tetraStiffness = this->calculateStiffnessMatrixForTetrahedron(tetraIndex);
                
                for(int tetraRowIndex = 0; tetraRowIndex < 4; ++tetraRowIndex) {
                    auto rowVertexIndex = this->mesh.getIndexOfVertexInTetrahedron(tetraIndex, tetraRowIndex) * 3;
                    
                    for(int tetraColIndex = 0; tetraColIndex < 4; ++tetraColIndex) {
                        auto colVertexIndex = this->mesh.getIndexOfVertexInTetrahedron(tetraIndex, tetraColIndex) * 3;
                        
                        for(int dimensionIndexRow = 0; dimensionIndexRow < 3; ++dimensionIndexRow) {
                            for(int dimensionIndexCol = 0; dimensionIndexCol < 3; ++dimensionIndexCol) {
                                auto coeff = tetraStiffness.coeffRef(tetraRowIndex + dimensionIndexRow, tetraColIndex + dimensionIndexCol);
                                if(coeff != 0) {
                                    stiffnessMatrix.coeffRef(rowVertexIndex + dimensionIndexRow, colVertexIndex + dimensionIndexCol)
                                            += coeff;
                                }
                            }
                        }
                    }
                }
            }
            
            stiffnessMatrix.makeCompressed();
            
            return stiffnessMatrix;
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
            TIMED_FUNC(timerCalcStiffnessMat);
            
            SparseMatrix<float, ColMajor> mat = this->calculateMassMatrix() + h * h * this->stiffnessMatrix + h * this->dampeningMatrix;
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
                                - this->stiffnessMatrix * this->calculateCurrentDifferenceFromRestPosition()
                                - this->dampeningMatrix * this->lastVelocities
                                - h * this->stiffnessMatrix * this->lastVelocities
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