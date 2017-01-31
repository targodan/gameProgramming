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
            return (this->youngsModulus / ((1+this->poissonsRatio)*(1-2*this->poissonsRatio)))
                    * materialMat;
        }
        
        SparseMatrix<double> DeformableBody::calculateStiffnessMatrixForTetrahedron(size_t index) const {
            MatrixXd A_inv(4, 4);
            A_inv << this->mesh.getTetrahedron(index).cast<double>(), MatrixXd::Ones(1, 4);
            
            auto A = A_inv.inverse();
            
            SparseMatrix<double> B(6, 12);
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
            
            
            return this->mesh.calculateVolumeOfTetrahedron(index) * B.transpose() * this->calculateMaterialMatrix() * B;
        }
        
        void DeformableBody::combine3by3Block(SparseMatrix<double>& target, int targetRow, int targetCol, const SparseMatrix<double>& source, int sourceRow, int sourceCol) const {
            for(int dimensionIndexRow = 0; dimensionIndexRow < 3; ++dimensionIndexRow) {
                for(int dimensionIndexCol = 0; dimensionIndexCol < 3; ++dimensionIndexCol) {
                    auto coeff = source.coeff(sourceRow + dimensionIndexRow, sourceCol + dimensionIndexCol);
                    if(coeff != double(0)) {
                        target.coeffRef(targetRow + dimensionIndexRow, targetCol + dimensionIndexCol) += coeff;
                    }
                }
            }
        }
        
        SparseMatrix<double> DeformableBody::calculateStiffnessMatrix() const {
            SparseMatrix<double> stiffnessMatrix(this->currentPosition.rows(), this->currentPosition.rows());
            stiffnessMatrix.reserve(12 * 12 + 63 * (this->mesh.getNumberOfTetrahedron()-1));
            
            for(size_t tetraIndex = 0; tetraIndex < this->mesh.getNumberOfTetrahedron(); ++tetraIndex) {
                auto tetraStiffness = this->calculateStiffnessMatrixForTetrahedron(tetraIndex);
                
                for(int tetraRowIndex = 0; tetraRowIndex < 4; ++tetraRowIndex) {
                    auto rowVertexIndex = this->mesh.getIndexOfVertexInTetrahedron(tetraIndex, tetraRowIndex) * 3;
                    
                    for(int tetraColIndex = 0; tetraColIndex < 4; ++tetraColIndex) {
                        auto colVertexIndex = this->mesh.getIndexOfVertexInTetrahedron(tetraIndex, tetraColIndex) * 3;
                        
                        combine3by3Block(stiffnessMatrix, rowVertexIndex, colVertexIndex, tetraStiffness, tetraRowIndex * 3, tetraColIndex * 3);
                    }
                }
            }
            
            stiffnessMatrix.makeCompressed();
            return stiffnessMatrix;
        }
        
        SparseMatrix<double> DeformableBody::calculateDampeningMatrix() const {
            SparseMatrix<double> dampening(this->currentPosition.rows(), this->currentPosition.rows());
            dampening.setIdentity();
            return this->dampening * dampening;
        }
        
        SparseMatrix<double> DeformableBody::calculateMassMatrix() const {
            SparseMatrix<double> mass(this->currentPosition.rows(), this->currentPosition.rows());
            for(int i = 0; i < this->currentPosition.rows(); ++i) {
                mass.insert(i, i) = static_cast<double>(this->mesh.getProperties().massPerVertex[i / 3]);
            }
            mass.makeCompressed();
            return mass;
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
            return this->currentPosition - this->restPosition;
        }
        
        VectorXd DeformableBody::calculateVelocities(float h, const VectorXf& forces) const {
            return this->lastVelocities +
                    (
                        this->stepMatrixSolver.solve(
                            h * (
                                forces.cast<double>()
                                - this->stiffnessMatrix * this->calculateCurrentDifferenceFromRestPosition()
                                - this->dampeningMatrix * this->lastVelocities
                                - this->stiffnessMatrix * h * this->lastVelocities
                            )
                        )
                    );
        }
        
        void DeformableBody::calculateAndSetInitialState(float targetStepSize) {
            // currentPosition points to the this->mesh.getSimulationMesh()
            this->restPosition = this->currentPosition;
            this->lastVelocities = VectorXd::Zero(this->currentPosition.rows());
            this->vertexFreezer = VectorXd::Ones(this->currentPosition.rows());
            this->dampeningMatrix = this->calculateDampeningMatrix();
            this->stiffnessMatrix = this->calculateStiffnessMatrix();
            this->updateStepMatrix(targetStepSize);
        }
        
        void DeformableBody::step(float deltaT, Force& force) {
            auto forces = force.getForceOnVertices(this->mesh.getProperties());
            if(forces.rows() > 0) {
                this->step(deltaT, forces);
            }
        }
        
        void DeformableBody::step(float deltaT, const VectorXf& forces) {
            this->updateStepMatrixIfNecessary(deltaT);
            this->lastVelocities = this->calculateVelocities(deltaT, forces);
            this->lastVelocities = this->lastVelocities.cwiseProduct(this->vertexFreezer);
            this->currentPosition += deltaT * this->lastVelocities;
            this->mesh.updateMeshFromPlanarVector(this->currentPosition);
        }
        
        const ObjectProperties& DeformableBody::getProperties() const {
            return this->mesh.getProperties();
        }
        
        VectorXd::Index DeformableBody::getExpectedForceVectorSize() const {
            return this->currentPosition.rows();
        }
        
        VectorXd& DeformableBody::getCurrentPosition() {
            return this->currentPosition;
        }
        
        const VectorXd& DeformableBody::getCurrentPosition() const {
            return this->currentPosition;
        }
        
        void DeformableBody::freezeVertex(size_t index) {
            this->vertexFreezer.segment<3>(index * 3) = Vector3d::Zero();
        }
        void DeformableBody::freezeVertices(const engine::util::Array<size_t>& indices) {
            for(auto index : indices) {
                this->vertexFreezer.segment<3>(index * 3) = Vector3d::Zero();
            }
        }
        void DeformableBody::unfreezeVertex(size_t index) {
            this->vertexFreezer.segment<3>(index * 3) = Vector3d::Ones();
        }
        void DeformableBody::unfreezeVertices(const engine::util::Array<size_t>& indices) {
            for(auto index : indices) {
                this->vertexFreezer.segment<3>(index * 3) = Vector3d::Ones();
            }
        }
    }
}