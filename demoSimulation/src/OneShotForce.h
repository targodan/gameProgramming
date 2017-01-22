#ifndef ONESHOTFORCE_H
#define ONESHOTFORCE_H

#include "engine/physics/Force.h"

#include <easylogging++.h>

namespace demoSimulation {
    class OneShotForce : public engine::physics::Force {
    protected:
        bool shotFired = false;
        
    public:
        OneShotForce() {}
        
        Eigen::Matrix<float, Eigen::Dynamic, 1> getForceOnVertices(const engine::physics::ObjectProperties& object) override {
            auto size = object.allVertices.rows();
            Eigen::Matrix<float, Eigen::Dynamic, 1> forces = Eigen::Matrix<float, Eigen::Dynamic, 1>::Zero(size, 1);
            if(!this->shotFired && this->secondsSinceStart > 0) {
                LOG(INFO) << "Boom.";
                forces(2) = -5;
                this->shotFired = true;
            }
            return forces;
        }
    };
}

#endif /* ONESHOTFORCE_H */

