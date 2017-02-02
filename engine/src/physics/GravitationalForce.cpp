#include "GravitationalForce.h"

namespace engine {
    namespace physics {
        GravitationalForce& GravitationalForce::operator=(const GravitationalForce& orig) {
            this->gravity = orig.gravity;
            this->centerOfGravity = orig.centerOfGravity;
            
            return *this;
        }
        GravitationalForce& GravitationalForce::operator=(GravitationalForce&& orig) {
            std::swap(this->gravity, orig.gravity);
            std::swap(this->centerOfGravity, orig.centerOfGravity);
            
            return *this;
        }

        VectorXf GravitationalForce::getForceOnVertices(const ObjectProperties& object) {
            VectorXf forces = object.allVertices;
            Map<MatrixXf> forcesInCols(forces.data(), 3, forces.rows() / 3);
            
            MatrixXf centers(3, forces.rows() / 3);
            for(int i = 0; i < forces.rows() / 3; ++i) {
                centers.col(i) = this->centerOfGravity;
            }
            
            forcesInCols -= centers;
            forcesInCols *= -1;
            forcesInCols.colwise().normalize();
            
            for(int col = 0; col < forcesInCols.cols(); ++col) {
                forcesInCols.col(col) *= object.massPerVertex[col] * this->gravity;
            }
            
            return forces;
        }
    }
}
