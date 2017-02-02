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
        
        Eigen::VectorXf getForceOnVertices(const engine::physics::ObjectProperties& object) override {
            auto size = object.allVertices.rows();
            Eigen::VectorXf forces = Eigen::VectorXf::Zero(size);
            if(!this->shotFired && this->secondsSinceStart > 0) {
                LOG(INFO) << "Boom.";
                forces(1) = 50;
                forces(2) = -50;
//                forces(10) = -700;
//                forces(11) = 700;
                this->shotFired = true;
            }
            return forces;
        }
    };
}

#endif /* ONESHOTFORCE_H */

