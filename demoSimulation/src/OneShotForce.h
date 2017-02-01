#ifndef ONESHOTFORCE_H
#define ONESHOTFORCE_H

#include "engine/physics/Force.h"

#include <easylogging++.h>

namespace demoSimulation {
    class OneShotForce : public engine::physics::Force {
    protected:
        bool shotFired;
        
    public:
        OneShotForce() : shotFired(true) {}
        
        Eigen::VectorXf getForceOnVertices(const engine::physics::ObjectProperties& object) override {
            auto size = object.allVertices.rows();
            Eigen::VectorXf forces = Eigen::VectorXf::Zero(size);
            if(!this->shotFired && this->secondsSinceStart > 0) {
                auto vertIndex = object.allVertices.rows() / 3 / 2;
                forces[vertIndex*3 + 2] = -6000;
                LOG(INFO) << "Boom.";
                this->shotFired = true;
            }
            return forces;
        }
        
        void resetShot() {
            this->shotFired = false;
        }
    };
}

#endif /* ONESHOTFORCE_H */

