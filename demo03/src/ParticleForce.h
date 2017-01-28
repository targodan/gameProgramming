/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ParticleForce.h
 * Author: markus
 *
 * Created on 26. Januar 2017, 16:54
 */

#ifndef PARTICLEFORCE_H
#define PARTICLEFORCE_H

#include "engine/physics/Force.h"

#include <easylogging++.h>
#include <random>
#include <glm/glm.hpp>

namespace demoSimulation {
    class ParticleForce {
    protected:
        bool shotFired = false;
        
        
    public:
        ParticleForce() {
        }
        
        static Eigen::Matrix<float, Eigen::Dynamic, 1> getForceOnVertices(int number) override {
            auto size = number;
            Eigen::Matrix<float, Eigen::Dynamic, 1> forces = Eigen::Matrix<float, Eigen::Dynamic, 1>::Zero(size, 1);
            if(true) {
                LOG(INFO) << "Boom.";
                
                std::default_random_engine generator;
                std::uniform_real_distribution<float> pitchDis(-90, 90);
                std::uniform_real_distribution<float> jawDis(0, 360);
                std::uniform_real_distribution<float> powDis(0, 1.1);
                float pitch = pitchDis(generator);
                float jaw = jawDis(generator);
                float pow = powDis(generator);
                for(int i = 0; i<size; i+=3){
                    forces(i,1) = 50*pow*glm::cos(jaw)*glm::cos(pitch) ;
                    forces(i+1,1) = 50*pow*glm::sin(pitch);
                    forces(i+2,1) = 50*pow*glm::sin(jaw)*glm::cos(pitch);
                }
                
            }
            return forces;
        }
    };
}


#endif /* PARTICLEFORCE_H */

