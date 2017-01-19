#ifndef ONESHOTFORCE_H
#define ONESHOTFORCE_H

#include "engine/physics/Force.h"

namespace demoSimulation {
    class OneShotForce : public engine::physics::Force {
    protected:
        float startTime;
        bool shotFired = false;
        
    public:
        OneShotForce(float startTime) : startTime(startTime) {}
        
        Eigen::Matrix<float, Eigen::Dynamic, 1> getForceOnVertices(const engine::physics::ObjectProperties& object) const override {
            Eigen::Matrix<float, Eigen::Dynamic, 1> forces = Eigen::Matrix<float, Eigen::Dynamic, 1>::Zero(object.allVertices.rows(), 1);
            if(!shotFired && this->secondsSinceStart >= this->startTime) {
                forces(2) = -5;
            }
            return forces;
        }
    };
}

#endif /* ONESHOTFORCE_H */

