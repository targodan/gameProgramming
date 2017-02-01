#ifndef ONESHOTFORCE_H
#define ONESHOTFORCE_H

#include "engine/physics/Force.h"

#include <easylogging++.h>

namespace demoSimulation {
    class OneShotForce : public engine::physics::Force {
    protected:
        bool shotFired;
        
    public:
        OneShotForce() : shotFired(false) {}
        
        Eigen::VectorXf getForceOnVertices(const engine::physics::ObjectProperties& object) override {
            auto size = object.allVertices.rows();
            Eigen::VectorXf forces = Eigen::VectorXf::Zero(size);
            if(!this->shotFired && this->secondsSinceStart > 0 && this->secondsSinceStart < 1) {
                auto vertIndex = object.allVertices.rows() / 3 / 2;
                forces[vertIndex*3 + 2] = -60000;
                LOG(INFO) << "Boom.";
                this->shotFired = true;
            }
            return forces;
        }
    };
}

#endif /* ONESHOTFORCE_H */

