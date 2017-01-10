#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "Force.h"

namespace engine {
    namespace physics {
        using namespace Eigen;
        
        class Explosion : public Force {
        private:
            Matrix<float, 3, 1> center;
            float initialSpeed; // in m/s
            float dampening;    // in m/s²
            
        public:
            Explosion(Matrix<float, 3, 1> center, float initialSpeed, float dampening)
                : center(center), initialSpeed(initialSpeed) {}
            
            Matrix<float, Dynamic, 1> getForceOnVertices(const Matrix<float, Dynamic, 1>& verticies) const override;
        };
    }
}

#endif /* EXPLOSION_H */

