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

#include <easylogging++.h>
#include <random>
#include <glm/glm.hpp>
#include "eigen3/Eigen/Eigen"

namespace demo {
    class ParticleForce {
    protected:
        bool shotFired = false;
        
        
    public:
        ParticleForce() {
        }
        static Eigen::Matrix<float, Eigen::Dynamic, 1> getForceOnVertices( const int number) {
            const int size = number;
            Eigen::Matrix<float, Eigen::Dynamic, 1> forces(size, 1);
            if(true) {
                int i;
                LOG(INFO) << "Boom.";
                
                std::default_random_engine generator;
                std::uniform_real_distribution<float> pitchDis(-90, 90);
                std::uniform_real_distribution<float> jawDis(0, 360);
                std::uniform_real_distribution<float> powDis(0, 1.1);
                float pitch;
                float jaw;
                float pow;
                forces(0,0) = 0;
                forces(1,0) = 0;
                forces(2,0) = 0;
                for(i = 3; i<size; i+=3){
                    pitch = pitchDis(generator);
                    jaw = jawDis(generator);
                    pow = powDis(generator);
                    forces(i,0) = 300*pow*glm::cos(jaw)*glm::cos(pitch) ;
                    forces(i+1,0) = 300*pow*glm::sin(pitch);
                    forces(i+2,0) = 300*pow*glm::sin(jaw)*glm::cos(pitch);
                }
                
            }
            return forces;
        }
    };
}


#endif /* PARTICLEFORCE_H */

