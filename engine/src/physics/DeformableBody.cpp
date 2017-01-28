#include "DeformableBody.h"

#include <easylogging++.h>

#ifndef ABS
#define ABS(x) ((x) < 0 ? -(x) : (x))
#endif

namespace engine {
    namespace physics {
        SparseMatrix<double> DeformableBody::calculateMaterialMatrix() const {
            SparseMatrix<double> materialMat(6, 6);
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
            auto ret = (this->youngsModulus / ((1+this->poissonsRatio)*(1-2*this->poissonsRatio)))
                    * materialMat;
            LOG(INFO) << "D" << std::endl << ret;
            return ret;
        }
        
        SparseMatrix<double> DeformableBody::calculateStiffnessMatrixForTetrahedron(size_t index) const {
            MatrixXf A_inv(4, 4);
            A_inv << this->mesh.getTetrahedron(index), MatrixXf::Ones(1, 4);
            
            LOG(INFO) << "MESH" << this->mesh.getTetrahedron(index);
            
            LOG(INFO) << "A_inv" << std::endl << A_inv;
            
            auto A = A_inv.inverse();
            
            LOG(INFO) << "A" << std::endl << A;
            
            auto B = SparseMatrix<double>(6, 12);
            B.reserve(36);
            
            for(int i = 0; i < 4; ++i) {
                auto baseInd = i * 3;
                B.insert(0, baseInd + 0) = A(i, 0);
                B.insert(1, baseInd + 1) = A(i, 1);
                B.insert(2, baseInd + 2) = A(i, 2);
                
                B.insert(3, baseInd + 0) = 0.5 * A(i, 1);
                B.insert(3, baseInd + 1) = 0.5 * A(i, 0);
                B.insert(4, baseInd + 1) = 0.5 * A(i, 2);
                B.insert(4, baseInd + 2) = 0.5 * A(i, 1);
                B.insert(5, baseInd + 0) = 0.5 * A(i, 2);
                B.insert(5, baseInd + 2) = 0.5 * A(i, 0);
            }
            
            LOG(INFO) << "B" << std::endl << B;
            
            return this->mesh.calculateVolumeOfTetrahedron(index) * B.transpose() * this->calculateMaterialMatrix() * B;
        }
        
        SparseMatrix<double> DeformableBody::calculateStiffnessMatrix() const {
//            SparseMatrix<float> stiffnessMatrix(this->properties.allVertices.rows(), this->properties.allVertices.rows());
//            stiffnessMatrix.reserve(12 * 12 + 63 * this->mesh.getNumberOfTetrahedron());
//            
//            for(size_t tetraIndex = 0; tetraIndex < this->mesh.getNumberOfTetrahedron(); ++tetraIndex) {
//                auto tetraStiffness = this->calculateStiffnessMatrixForTetrahedron(tetraIndex);
//                
//                for(int tetraRowIndex = 0; tetraRowIndex < 4; ++tetraRowIndex) {
//                    auto rowVertexIndex = this->mesh.getIndexOfVertexInTetrahedron(tetraIndex, tetraRowIndex) * 3;
//                    
//                    for(int tetraColIndex = 0; tetraColIndex < 4; ++tetraColIndex) {
//                        auto colVertexIndex = this->mesh.getIndexOfVertexInTetrahedron(tetraIndex, tetraColIndex) * 3;
//                        
//                        for(int dimensionIndexRow = 0; dimensionIndexRow < 3; ++dimensionIndexRow) {
//                            for(int dimensionIndexCol = 0; dimensionIndexCol < 3; ++dimensionIndexCol) {
//                                auto coeff = tetraStiffness.coeffRef(tetraRowIndex + dimensionIndexRow, tetraColIndex + dimensionIndexCol);
//                                if(coeff != 0) {
//                                    stiffnessMatrix.coeffRef(rowVertexIndex + dimensionIndexRow, colVertexIndex + dimensionIndexCol)
//                                            += coeff;
//                                }
//                            }
//                        }
//                    }
//                }
//            }
//            
//            stiffnessMatrix.makeCompressed();
//            
//            return stiffnessMatrix;
            auto stiff = this->calculateStiffnessMatrixForTetrahedron(0);
            stiff.makeCompressed();
            return stiff;
        }
        
        SparseMatrix<double> DeformableBody::calculateDampeningMatrix() const {
            SparseMatrix<double> dampening(this->properties.allVertices.rows(), this->properties.allVertices.rows());
            dampening.setIdentity();
            return this->dampening * dampening;
        }
        
        SparseMatrix<double> DeformableBody::calculateMassMatrix() const {
            SparseMatrix<double> mass(this->properties.allVertices.rows(), this->properties.allVertices.rows());
            mass.setIdentity();
            return this->mass * mass;
        }
        
        SparseMatrix<double, ColMajor> DeformableBody::calculateStepMatrix(float h) const {
            SparseMatrix<double, ColMajor> mat = this->calculateMassMatrix() + h * h * this->stiffnessMatrix + h * this->dampeningMatrix;
            mat.makeCompressed();
            return mat;
        }
        
        void DeformableBody::prepareStepMatrixSolver() {
            this->stepMatrixSolver.analyzePattern(this->stepMatrix);
            this->stepMatrixSolver.factorize(this->stepMatrix);
        }
            
        void DeformableBody::updateStepMatrix(float h) {
            TIMED_FUNC(timerCalcStiffnessMat);
            
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
        
        VectorXd DeformableBody::calculateCurrentDifferenceFromRestPosition() const {
            return this->currentPosition.cast<double>() - this->restPosition.cast<double>();
        }
        
        VectorXd DeformableBody::calculateVelocities(float h, const VectorXf& forces) const {
            LOG(INFO) << "Diff to Rest" << std::endl << this->calculateCurrentDifferenceFromRestPosition();
            LOG(INFO) << "Rückstellkraft" << std::endl << this->stiffnessMatrix * this->calculateCurrentDifferenceFromRestPosition();
            return this->lastVelocities +
                    (
                        this->stepMatrixSolver.solve(
                            h * (
                                forces.cast<double>()
                                - this->stiffnessMatrix * this->calculateCurrentDifferenceFromRestPosition()
                                - this->dampeningMatrix * this->lastVelocities
//                                - this->stiffnessMatrix * h * this->lastVelocities
                            )
                        )
                    );
        }
        
        void DeformableBody::calculateAndSetInitialState(float targetStepSize) {
            // currentPosition points to the objectProperties->allVertices
            this->restPosition = this->currentPosition;
            this->lastVelocities = VectorXd::Zero(this->currentPosition.rows());
            this->dampeningMatrix = this->calculateDampeningMatrix();
            this->stiffnessMatrix = this->calculateStiffnessMatrix();
            LOG(INFO) << "K" << std::endl << this->stiffnessMatrix;
            this->updateStepMatrix(targetStepSize);
        }
        
        void DeformableBody::step(float deltaT, Force& force) {
            auto forces = force.getForceOnVertices(this->properties);
            if(forces.rows() > 0) {
                this->step(deltaT, forces);
            }
        }
        
        void DeformableBody::step(float deltaT, const VectorXf& forces) {
            LOG(INFO) << "REST" << std::endl << this->restPosition;
            this->updateStepMatrixIfNecessary(deltaT);
            this->lastVelocities = this->calculateVelocities(deltaT, forces);
            this->currentPosition += deltaT * this->lastVelocities.cast<float>();
            this->mesh.updateMeshFromPlanarVector(this->currentPosition);
        }
        
        const ObjectProperties& DeformableBody::getProperties() const {
            return this->properties;
        }
        
        VectorXf::Index DeformableBody::getExpectedForceVectorSize() const {
            return this->currentPosition.rows();
        }
        
        VectorXf& DeformableBody::getCurrentPosition() {
            return this->currentPosition;
        }
        
        const VectorXf& DeformableBody::getCurrentPosition() const {
            return this->currentPosition;
        }
    }
}