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
        
        std::pair<SparseMatrix<double>, MatrixXd> DeformableBody::calculateStiffnessAndStressMatrixForTetrahedron(size_t index) const {
            MatrixXd A_inv(4, 4);
            auto tetrahedron = this->getRestTetrahedron(index);
            A_inv << tetrahedron.cast<double>(), MatrixXd::Ones(1, 4);
            
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
            
            SparseMatrix<double> stressMatrix = this->calculateMaterialMatrix() * B;
            
            float volume = abs(
                        (static_cast<Vector3f>(tetrahedron.col(1) - tetrahedron.col(0)))
                        .cross
                        (static_cast<Vector3f>(tetrahedron.col(2) - tetrahedron.col(0)))
                        .dot(static_cast<Vector3f>(tetrahedron.col(3) - tetrahedron.col(0)))
                    ) / 6;
            
            return std::make_pair(volume * B.transpose() * stressMatrix, MatrixXd(stressMatrix));
        }
        
        void DeformableBody::combineBlock(int blockRows, int blockCols, SparseMatrix<double>& target, int targetRow, int targetCol, const SparseMatrix<double>& source, int sourceRow, int sourceCol) const {
            for(int dimensionIndexRow = 0; dimensionIndexRow < blockRows; ++dimensionIndexRow) {
                for(int dimensionIndexCol = 0; dimensionIndexCol < blockCols; ++dimensionIndexCol) {
                    auto coeff = source.coeff(sourceRow + dimensionIndexRow, sourceCol + dimensionIndexCol);
                    if(coeff != double(0)) {
                        target.coeffRef(targetRow + dimensionIndexRow, targetCol + dimensionIndexCol) += coeff;
                    }
                }
            }
        }
        void DeformableBody::combineBlock(int blockRows, int blockCols, SparseMatrix<double>& target, int targetRow, int targetCol, const MatrixXd& source, int sourceRow, int sourceCol) const {
            for(int dimensionIndexRow = 0; dimensionIndexRow < blockRows; ++dimensionIndexRow) {
                for(int dimensionIndexCol = 0; dimensionIndexCol < blockCols; ++dimensionIndexCol) {
                    auto coeff = source(sourceRow + dimensionIndexRow, sourceCol + dimensionIndexCol);
                    if(coeff != double(0)) {
                        target.coeffRef(targetRow + dimensionIndexRow, targetCol + dimensionIndexCol) += coeff;
                    }
                }
            }
        }
        
        void DeformableBody::combineStiffnessMatrices(SparseMatrix<double>& target, const SparseMatrix<double>& source, size_t tetraIndex) const {
            for(int tetraRowIndex = 0; tetraRowIndex < 4; ++tetraRowIndex) {
                auto rowVertexIndex = this->mesh.getIndexOfVertexInTetrahedron(tetraIndex, tetraRowIndex) * 3;

                for(int tetraColIndex = 0; tetraColIndex < 4; ++tetraColIndex) {
                    auto colVertexIndex = this->mesh.getIndexOfVertexInTetrahedron(tetraIndex, tetraColIndex) * 3;

                    this->combineBlock(3, 3, target, rowVertexIndex, colVertexIndex, source, tetraRowIndex * 3, tetraColIndex * 3);
                }
            }
        }
        
        std::pair<SparseMatrix<double>, SparseMatrix<double>> DeformableBody::calculateStiffnessAndStressMatrix() const {
            SparseMatrix<double> stiffnessMatrix(this->mesh.getSimulationMesh().rows(), this->mesh.getSimulationMesh().rows());
            stiffnessMatrix.reserve(12 * 12 + 63 * (this->mesh.getNumberOfTetrahedron()-1));
            
            SparseMatrix<double> stressMatrix(this->mesh.getNumberOfTetrahedron(), this->mesh.getSimulationMesh().rows());
            if(this->isBreakingEnabled()) {
                // TODO: Reserve on stressMatrix
            }
            
            for(size_t tetraIndex = 0; tetraIndex < this->mesh.getNumberOfTetrahedron(); ++tetraIndex) {
                auto tetraMatrices = this->calculateStiffnessAndStressMatrixForTetrahedron(tetraIndex);
                
                this->combineStiffnessMatrices(stiffnessMatrix, tetraMatrices.first, tetraIndex);
                
                if(this->isBreakingEnabled()) {
                    auto tetraStress = tetraMatrices.second.colwise().sum();
                    for(int tetraColIndex = 0; tetraColIndex < 4; ++tetraColIndex) {
                        auto rowIndex = tetraIndex;
                        auto colVertexIndex = this->mesh.getIndexOfVertexInTetrahedron(tetraIndex, tetraColIndex) * 3;

                        this->combineBlock(1, 3, stressMatrix, rowIndex, colVertexIndex, tetraStress, 0, tetraColIndex * 3);
                    }
                }
            }
            
            stiffnessMatrix.makeCompressed();
            stressMatrix.makeCompressed();
            
            LOG(INFO) << "big stress "  << stressMatrix.rows() << "x" << stressMatrix.cols();
            
            return std::make_pair(stiffnessMatrix, stressMatrix);
        }
        
        SparseMatrix<double> DeformableBody::calculateDampeningMatrix() const {
            SparseMatrix<double> dampening(this->mesh.getSimulationMesh().rows(), this->mesh.getSimulationMesh().rows());
            dampening.setIdentity();
            return this->dampening * dampening;
        }
        
        SparseMatrix<double> DeformableBody::calculateMassMatrix() const {
            SparseMatrix<double> mass(this->mesh.getSimulationMesh().rows(), this->mesh.getSimulationMesh().rows());
            for(int i = 0; i < this->mesh.getSimulationMesh().rows(); ++i) {
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
            this->stepMatrixSolver.compute(this->stepMatrix);
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
            return (this->mesh.getSimulationMesh() - this->restPosition).cast<double>();
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
            LOG(INFO) << "Setting up deformable body matrices...";
            this->restPosition = this->mesh.getSimulationMesh();
            this->lastVelocities = VectorXd::Zero(this->mesh.getSimulationMesh().rows());
            this->vertexFreezer = VectorXd::Ones(this->mesh.getSimulationMesh().rows());
            this->dampeningMatrix = this->calculateDampeningMatrix();
            auto materialbasedMatrices = this->calculateStiffnessAndStressMatrix();
            this->stiffnessMatrix = materialbasedMatrices.first;
            if(this->isBreakingEnabled()) {
                this->stressMatrix = materialbasedMatrices.second;
            }
            this->updateStepMatrix(targetStepSize);
            LOG(INFO) << "Done. Size of K: " << this->stiffnessMatrix.rows() << "x" << this->stiffnessMatrix.cols();
        }
        
        void DeformableBody::step(float deltaT, Force& force) {
            auto forces = force.getForceOnVertices(this->mesh.getProperties());
            if(forces.rows() > 0) {
                this->step(deltaT, forces);
            }
        }
        
        void DeformableBody::step(float deltaT, const VectorXf& forces) {
            TIMED_FUNC_IF(timerSimulationStep, VLOG_IS_ON(1));
            this->updateStepMatrixIfNecessary(deltaT);
            this->lastVelocities = this->calculateVelocities(deltaT, forces);
            // LOG(INFO) << "Error: " << this->stepMatrixSolver.error();
            this->lastVelocities = this->lastVelocities.cwiseProduct(this->vertexFreezer);
            this->mesh.updateMeshFromPlanarVector(this->mesh.getSimulationMesh() + (deltaT * this->lastVelocities).cast<float>());
            if(this->isBreakingEnabled()) {
                this->breakOnHighStress();
            }
        }
        
        VectorXd DeformableBody::calculateStressPerTetrahedron(const VectorXd& deformation) {
            return (this->stressMatrix * deformation).cwiseAbs();
        }
        
        void DeformableBody::breakOnHighStress() {
            VectorXd stress = this->calculateStressPerTetrahedron(this->calculateCurrentDifferenceFromRestPosition());
            
            LOG(INFO) << "Avg stress:" << stress.sum() / stress.rows();
            
            Set<size_t> breakingTetrahedra;
            breakingTetrahedra.set_empty_key(SIZE_MAX);
            
            vector<std::pair<size_t, size_t>> breakingEdges;
            for(int tetraIndex = 0; tetraIndex < stress.rows(); ++tetraIndex) {
                if(stress[tetraIndex] > this->stressThresholdSqForBreaking) {
                    auto breakingEdge = this->findBreakingEdgeOfTetrahedron(tetraIndex);
                    if(this->deletedEdges.find(breakingEdge) == this->deletedEdges.end()) {
                        breakingEdges.push_back(breakingEdge);
                        this->deletedEdges.insert(breakingEdge);
                        this->deletedEdges.insert(std::make_pair(breakingEdge.second, breakingEdge.first));

                        auto tetrahedra = this->findTetrahedraAdjacentToEdge(breakingEdge);
                        breakingTetrahedra.insert(tetrahedra.begin(), tetrahedra.end());
                    }
                }
            }
            
            if(breakingEdges.size() > 0) {
                this->mesh.deleteEdges(breakingEdges);

                for(auto index : breakingTetrahedra) {
                    this->deleteTetrahedronFromStiffnessAndStress(index);
                }

                this->prepareStepMatrixSolver();
            
                // batch-remove all 0es
                this->stressMatrix.prune([](auto row, auto col, auto val) { return abs(val) > 1e-16; });
            }
        }

        vector<size_t> DeformableBody::findTetrahedraAdjacentToEdge(const std::pair<size_t, size_t>& edge) const {
            vector<size_t> tetrahedra;
            
            const auto& indices = this->mesh.getTetrahedronIndices();
            
            for(size_t tetraIndex = 0; tetraIndex < indices.size() / 4; ++tetraIndex) {
                if(this->deletedTetrahedra.find(tetraIndex) != this->deletedTetrahedra.end()) {
                    continue;
                }
                if(
                           (indices[tetraIndex * 4 + 0] == edge.first && indices[tetraIndex * 4 + 1] == edge.second)
                        || (indices[tetraIndex * 4 + 0] == edge.first && indices[tetraIndex * 4 + 2] == edge.second)
                        || (indices[tetraIndex * 4 + 0] == edge.first && indices[tetraIndex * 4 + 3] == edge.second)
                        || (indices[tetraIndex * 4 + 1] == edge.first && indices[tetraIndex * 4 + 2] == edge.second)
                        || (indices[tetraIndex * 4 + 1] == edge.first && indices[tetraIndex * 4 + 3] == edge.second)
                        || (indices[tetraIndex * 4 + 2] == edge.first && indices[tetraIndex * 4 + 3] == edge.second)
                        || (indices[tetraIndex * 4 + 0] == edge.second && indices[tetraIndex * 4 + 1] == edge.first)
                        || (indices[tetraIndex * 4 + 0] == edge.second && indices[tetraIndex * 4 + 2] == edge.first)
                        || (indices[tetraIndex * 4 + 0] == edge.second && indices[tetraIndex * 4 + 3] == edge.first)
                        || (indices[tetraIndex * 4 + 1] == edge.second && indices[tetraIndex * 4 + 2] == edge.first)
                        || (indices[tetraIndex * 4 + 1] == edge.second && indices[tetraIndex * 4 + 3] == edge.first)
                        || (indices[tetraIndex * 4 + 2] == edge.second && indices[tetraIndex * 4 + 3] == edge.first)
                    ) {
                    tetrahedra.push_back(tetraIndex);
                }
            }
            return tetrahedra;
        }
        
        std::pair<size_t, size_t> DeformableBody::findBreakingEdgeOfTetrahedron(size_t tetraIndex) const {
            MatrixXf currentTetrahedron = this->mesh.getTetrahedron(tetraIndex);
            MatrixXf restTetrahedron = this->getRestTetrahedron(tetraIndex);
            
            engine::util::Array<std::pair<size_t, size_t>> edges = {
                {0, 1},
                {0, 2},
                {0, 3},
                {1, 2},
                {1, 3},
                {2, 3}
            };
            
            MatrixXf restFrom(3, 6);
            restFrom << restTetrahedron.col(0), restTetrahedron.col(0), restTetrahedron.col(0), restTetrahedron.col(1), restTetrahedron.col(1), restTetrahedron.col(2);
            MatrixXf restTo(3, 6);
            restTo << restTetrahedron.col(1), restTetrahedron.col(2), restTetrahedron.col(3), restTetrahedron.col(2), restTetrahedron.col(3), restTetrahedron.col(3);
            
            MatrixXf currentFrom(3, 6);
            currentFrom << currentTetrahedron.col(0), currentTetrahedron.col(0), currentTetrahedron.col(0), currentTetrahedron.col(1), currentTetrahedron.col(1), currentTetrahedron.col(2);
            MatrixXf currentTo(3, 6);
            currentTo << currentTetrahedron.col(1), currentTetrahedron.col(2), currentTetrahedron.col(3), currentTetrahedron.col(2), currentTetrahedron.col(3), currentTetrahedron.col(3);
            
            VectorXf sqEdgeLengthDifferences = (
                        (restFrom - restTo).colwise().squaredNorm()
                       -(currentFrom - currentTo).colwise().squaredNorm()
                    ).cwiseAbs().transpose();
            
            std::pair<int, int> maxEdge = edges[0];
            float maxDiff = sqEdgeLengthDifferences[0];
            for(int i = 1; i < sqEdgeLengthDifferences.rows(); ++i) {
                if(maxDiff < sqEdgeLengthDifferences[i]) {
                    maxDiff = sqEdgeLengthDifferences[i];
                    maxEdge = edges[i];
                }
            }
            return std::make_pair(
                    this->mesh.getIndexOfVertexInTetrahedron(tetraIndex, maxEdge.first),
                    this->mesh.getIndexOfVertexInTetrahedron(tetraIndex, maxEdge.second)
                    );
        }
        
        void DeformableBody::deleteTetrahedronFromStiffnessAndStress(size_t tetraIndex) {
            if(this->deletedTetrahedra.find(tetraIndex) != this->deletedTetrahedra.end()) {
                return;
            }
            LOG(INFO) << "Deleted " << tetraIndex;
            // Delete from stress
            for(size_t row = tetraIndex; row < tetraIndex+1; ++row) {
                for(int col = 0; col < this->stressMatrix.cols(); ++col) {
                    if(this->stressMatrix.coeff(row, col) != double(0)) {
                        this->stressMatrix.coeffRef(row, col) = 0;
                    }
                }
            }
            this->combineStiffnessMatrices(this->stiffnessMatrix, -this->calculateStiffnessAndStressMatrixForTetrahedron(tetraIndex).first, tetraIndex);
            this->deletedTetrahedra.insert(tetraIndex);
        }
        
        const ObjectProperties& DeformableBody::getProperties() const {
            return this->mesh.getProperties();
        }
        
        VectorXd::Index DeformableBody::getExpectedForceVectorSize() const {
            return this->mesh.getSimulationMesh().rows();
        }
        
        const VectorXf& DeformableBody::getCurrentPosition() const {
            return this->mesh.getSimulationMesh();
        }
        
        Vector3f DeformableBody::getVertexOfRestPosition(int index) const {
            return this->restPosition.segment<3>(index * 3);
        }
        
        MatrixXf DeformableBody::getRestTetrahedron(int tetraIndex) const {
            MatrixXf tetrahedron(3, 4);
            tetrahedron.col(0) = this->getVertexOfRestPosition(this->mesh.getIndexOfVertexInTetrahedron(tetraIndex, 0));
            tetrahedron.col(1) = this->getVertexOfRestPosition(this->mesh.getIndexOfVertexInTetrahedron(tetraIndex, 1));
            tetrahedron.col(2) = this->getVertexOfRestPosition(this->mesh.getIndexOfVertexInTetrahedron(tetraIndex, 2));
            tetrahedron.col(3) = this->getVertexOfRestPosition(this->mesh.getIndexOfVertexInTetrahedron(tetraIndex, 3));
            return tetrahedron;
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
        void DeformableBody::unfreezeAllVertices() {
            this->vertexFreezer = VectorXd::Ones(this->vertexFreezer.rows());
        }
        
        bool DeformableBody::isBreakingEnabled() const {
            return this->stressThresholdSqForBreaking > 0;
        }
        
        void DeformableBody::resetSimulation() {
            this->lastVelocities = VectorXd::Zero(this->lastVelocities.rows());
            this->mesh.updateMeshFromPlanarVector(this->restPosition);
        }
    }
}
