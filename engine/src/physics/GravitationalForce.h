#ifndef GRAVITATIONALFORCE_H
#define GRAVITATIONALFORCE_H

#include "Force.h"
#include "constants.h"

namespace engine {
    namespace physics {
        using namespace Eigen;
        
        class GravitationalForce : public Force {
        protected:
            float gravity;
            Vector3f centerOfGravity;
            
        public:
            GravitationalForce(float gravity = GRAVITY_ON_EARTH, Vector3f centerOfGravity = Vector3f(0, -6371e3f, 0))
                : gravity(gravity), centerOfGravity(centerOfGravity) {}
            GravitationalForce(const GravitationalForce& orig)
                : gravity(orig.gravity), centerOfGravity(orig.centerOfGravity) {}
            GravitationalForce(GravitationalForce&& orig)
                : gravity(std::move(orig.gravity)), centerOfGravity(std::move(centerOfGravity)) {}
            
            GravitationalForce& operator=(const GravitationalForce& orig);
            GravitationalForce& operator=(GravitationalForce&& orig);
            
            VectorXf getForceOnVertices(const ObjectProperties& object) override;
        };
    }
}

#endif /* GRAVITATIONALFORCE_H */

