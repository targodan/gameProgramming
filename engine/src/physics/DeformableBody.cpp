#include "DeformableBody.h"

#ifndef ABS
#define ABS(x) ((x) < 0 ? -(x) : (x))
#endif

namespace engine {
    namespace physics {
        Vector<12> DeformableBody::calculatePlanarVectorsFromMesh() const {
            Vector<12> planarVectors;
            size_t i = 0;
            for(const auto& v : this->mesh.getVertices()) {
                planarVectors[i*3 + 0] = v.position.x;
                planarVectors[i*3 + 1] = v.position.y;
                planarVectors[i*3 + 2] = v.position.z;
            }
            return planarVectors;
        }
        
        void DeformableBody::setMeshFromPlanarVectors(const Vector<12>& v) {
            for(size_t i = 0; i < v.getDimension(); i += 3) {
                this->mesh.getVertices()[i / 3].position.x = v[i+0];
                this->mesh.getVertices()[i / 3].position.y = v[i+1];
                this->mesh.getVertices()[i / 3].position.z = v[i+2];
            }
        }

        MatrixSq<6> DeformableBody::calculateMaterialMatrix() const {
            return (this->youngsModulus / (1+this->poissonsRatio)*(1-2*this->poissonsRatio))
                    * MatrixSq<6>({1 - this->poissonsRatio, this->poissonsRatio, this->poissonsRatio, 0, 0, 0, 
                    this->poissonsRatio, 1-this->poissonsRatio, this->poissonsRatio, 0, 0, 0,
                    this->poissonsRatio, this->poissonsRatio, 1-this->poissonsRatio, 0, 0, 0,
                    0, 0, 0, 1-2*this->poissonsRatio, 0, 0,
                    0, 0, 0, 0, 1-2*this->poissonsRatio, 0,
                    0, 0, 0, 0, 0, 1-2*this->poissonsRatio});
        }
        
        MatrixSq<12> DeformableBody::calculateStiffnessMatrix() const {
            // TODO: Check if A is made up of the rest position or the current one.
            auto& v = this->mesh.getVertices();
            auto A = MatrixSq<4>({
                v[0].position.x, v[1].position.x, v[2].position.x, v[3].position.x,
                v[0].position.y, v[1].position.y, v[2].position.y, v[3].position.y,
                v[0].position.z, v[1].position.z, v[2].position.z, v[3].position.z,
                1, 1, 1, 1
            });
            
            auto B = Matrix<12, 6>({
                A[0][0], 0, 0, A[1][0], 0, 0, A[2][0], 0, 0, A[3][0], 0, 0,
                0, A[0][1], 0, 0, A[1][1], 0, 0, A[2][1], 0, 0, A[3][1], 0,
                0, 0, 0, A[0][2], 0, 0, A[1][2], 0, 0, A[2][2], 0, 0, A[3][2],
                A[0][1], A[0][0], 0, A[1][1], A[1][0], 0, A[2][1], A[2][0], 0, A[3][1], A[3][0], 0,
                0, A[0][2], A[0][1], 0, A[1][2], A[1][1], 0, A[2][2], A[2][1], 0, A[3][2], A[3][1],
                A[0][2], 0, A[0][0], A[1][2], 0, A[1][0], A[2][2], 0, A[2][0], A[3][2], 0, A[3][0]
            });
            auto BT = B.transpose();
            
            // TODO: Check if multiply by volume!
            return BT * this->calculateMaterialMatrix() * B;
        }
        
        MatrixSq<12> DeformableBody::calculateDampeningMatrix() const {
            return this->dampening * MatrixSq<12>::identity();
        }
        
        MatrixSq<12> DeformableBody::calculateMassMatrix() const {
            return this->mass * MatrixSq<12>::identity();
        }
        
        MatrixSq<12> DeformableBody::calculateStepMatrix(float h) const {
            MatrixSq<12> mat = this->calculateMassMatrix() + h * (h * this->stiffnessMatrix + this->dampeningMatrix);
            return mat.inverse();
        }
            
        void DeformableBody::updateStepMatrixIfNecessary(float h) {
            if(this->stepSizeOnMatrixCalculation == 0
                    || ABS(this->stepSizeOnMatrixCalculation - h) / (float)this->stepSizeOnMatrixCalculation >= this->stepSizeDeviationPercentage) {
                this->stepMatrix = this->calculateStepMatrix(h);
                this->stepSizeOnMatrixCalculation = h;
            }
        }
        
        Vector<12> DeformableBody::calculateCurrentDifferenceFromRestPosition() const {
            return this->currentPosition - this->restPosition;
        }
        
        Vector<12> DeformableBody::calculateVelocities(float h, Vector<12> forces) const {
            return this->lastVelocities +
                    (
                        h * this->stepMatrix *
                        (
                            forces
                            - this->stiffnessMatrix * this->calculateCurrentDifferenceFromRestPosition()
                            - this->dampeningMatrix * this->lastVelocities
                        )
                    );
        }
        
        void DeformableBody::step(float h, Vector<12> forces) {
            this->updateStepMatrixIfNecessary(h);
            this->lastVelocities = this->calculateVelocities(h, forces);
            this->currentPosition += h * this->lastVelocities;
            this->setMeshFromPlanarVectors(this->currentPosition);
            this->mesh.loadMesh();
        }
    }
}