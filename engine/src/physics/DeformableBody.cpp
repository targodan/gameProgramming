#include "DeformableBody.h"

#ifndef ABS
#define ABS(x) ((x) < 0 ? -(x) : (x))
#endif

namespace engine {
    namespace physics {
        Matrix<float, 12, 1> DeformableBody::calculatePlanarVectorsFromMesh() const {
            Matrix<float, 12, 1> planarVectors;
            size_t i = 0;
            for(const auto& v : this->mesh.getVertices()) {
                planarVectors[i*3 + 0] = v.position.x;
                planarVectors[i*3 + 1] = v.position.y;
                planarVectors[i*3 + 2] = v.position.z;
                ++i;
            }
            return planarVectors;
        }
        
        void DeformableBody::setMeshFromPlanarVectors(const Matrix<float, 12, 1>& v) {
            for(size_t i = 0; i < 12; i += 3) {
                this->mesh.getVertices()[i / 3].position.x = v[i+0];
                this->mesh.getVertices()[i / 3].position.y = v[i+1];
                this->mesh.getVertices()[i / 3].position.z = v[i+2];
            }
        }

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
        
        float DeformableBody::calculateVolume() const {
            const Eigen::Map<MatrixXf> vertices(const_cast<float*>(this->currentPosition.data()), 3, this->currentPosition.rows() / 3);
            
            Vector3f normal = ((Vector3f)vertices.col(0)).cross((Vector3f)vertices.col(1));
            float baseArea = normal.norm();
            normal /= baseArea;
            
            Hyperplane<float, 3> basePlane(normal, vertices.row(0));
            return baseArea * basePlane.absDistance(vertices.row(3)) / 3.;
        }
        
        SparseMatrix<float> DeformableBody::calculateStiffnessMatrix() const {
            // TODO: Check if A is made up of the rest position or the current one.
            auto& v = this->mesh.getVertices();
            float buffer[16] = {
                v[0].position.x, v[1].position.x, v[2].position.x, v[3].position.x,
                v[0].position.y, v[1].position.y, v[2].position.y, v[3].position.y,
                v[0].position.z, v[1].position.z, v[2].position.z, v[3].position.z,
                1, 1, 1, 1
            };
            auto A = Matrix<float, 4, 4>(buffer).inverse();
            
            auto B = SparseMatrix<float>(6, 12);
            B.reserve(0);
            B.insert(0, 0)  = A(0, 0);
            B.insert(0, 3)  = A(1, 0);
            B.insert(0, 6)  = A(2, 0);
            B.insert(0, 9)  = A(3, 0);
            
            B.insert(1, 1)  = A(0, 1);
            B.insert(1, 4)  = A(1, 1);
            B.insert(1, 7)  = A(2, 1);
            B.insert(1, 10) = A(3, 1);
            
            B.insert(2, 2)  = A(0, 2);
            B.insert(2, 5)  = A(1, 2);
            B.insert(2, 8)  = A(2, 2);
            B.insert(2, 11) = A(3, 2);
            
            B.insert(3, 0)  = A(0, 1);
            B.insert(3, 1)  = A(0, 0);
            B.insert(3, 3)  = A(1, 1);
            B.insert(3, 4)  = A(1, 0);
            B.insert(3, 6)  = A(2, 1);
            B.insert(3, 7)  = A(2, 0);
            B.insert(3, 9)  = A(3, 1);
            B.insert(3, 10) = A(3, 0);
            
            B.insert(4, 1)  = A(0, 2);
            B.insert(4, 2)  = A(0, 1);
            B.insert(4, 4)  = A(1, 2);
            B.insert(4, 5)  = A(1, 1);
            B.insert(4, 7)  = A(2, 2);
            B.insert(4, 8)  = A(2, 1);
            B.insert(4, 10) = A(3, 2);
            B.insert(4, 11) = A(3, 1);
            
            B.insert(5, 0)  = A(0, 2);
            B.insert(5, 2)  = A(0, 0);
            B.insert(5, 3)  = A(1, 2);
            B.insert(5, 5)  = A(1, 0);
            B.insert(5, 6)  = A(2, 2);
            B.insert(5, 8)  = A(2, 0);
            B.insert(5, 9)  = A(3, 2);
            B.insert(5, 11) = A(3, 0);
            
            return this->calculateVolume() * B.transpose() * this->calculateMaterialMatrix() * B;
        }
        
        SparseMatrix<float> DeformableBody::calculateDampeningMatrix() const {
            SparseMatrix<float> dampening(12, 12);
            dampening.setIdentity();
            return this->dampening * dampening;
        }
        
        SparseMatrix<float> DeformableBody::calculateMassMatrix() const {
            SparseMatrix<float> mass(12, 12);
            mass.setIdentity();
            return this->mass * mass;
        }
        
        HouseholderQR<Matrix<float, 12, 12>> DeformableBody::calculateStepMatrix(float h) const {
            Matrix<float, 12, 12> mat = this->calculateMassMatrix() + h * (h * this->stiffnessMatrix + this->dampeningMatrix);
            return mat.householderQr();
        }
            
        void DeformableBody::updateStepMatrixIfNecessary(float h) {
            if(this->stepSizeOnMatrixCalculation == 0
                    || ABS(this->stepSizeOnMatrixCalculation - h) / (float)this->stepSizeOnMatrixCalculation >= this->stepSizeDeviationPercentage) {
                this->stepMatrix = this->calculateStepMatrix(h);
                this->stepSizeOnMatrixCalculation = h;
            }
        }
        
        Matrix<float, 12, 1> DeformableBody::calculateCurrentDifferenceFromRestPosition() const {
            return this->currentPosition - this->restPosition;
        }
        
        Matrix<float, 12, 1> DeformableBody::calculateVelocities(float h, const Matrix<float, 12, 1>& forces) const {
            return this->lastVelocities +
                    (
                        this->stepMatrix.solve(
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
            this->restPosition = this->calculatePlanarVectorsFromMesh();
            this->currentPosition = this->restPosition;
            this->dampeningMatrix = this->calculateDampeningMatrix();
            this->stiffnessMatrix = this->calculateStiffnessMatrix();
            this->updateStepMatrixIfNecessary(targetStepSize);
        }
        
        void DeformableBody::step(float h, const Force& force) {
            // TODO: Use engine::physics::Surface here
//            this->step(h, force.getForceOnVertices(this->currentPosition));
        }
        
        void DeformableBody::step(float h, const Matrix<float, 12, 1>& forces) {
            this->updateStepMatrixIfNecessary(h);
            this->lastVelocities = this->calculateVelocities(h, forces);
            this->currentPosition += h * this->lastVelocities;
            this->setMeshFromPlanarVectors(this->currentPosition);
            this->mesh.loadMesh();
        }
    }
}