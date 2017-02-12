/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ParticleSystemSystem.cpp
 * Author: markus
 * 
 * Created on 27. Januar 2017, 10:31
 */

#include "ParticleSystemSystem.h"
#include "../ECS/SystemRegisterer.h"
#include "../ECSCommon/ParticleSystemComponent.h"
#include <eigen3/Eigen/Eigen>

namespace engine {
    namespace ECSCommon {  
                using namespace Eigen;

        ECS_REGISTER_SYSTEM(ParticleSystemSystem);
        
        systemId_t ParticleSystemSystem::systemId = 0;
        
        ParticleSystemSystem::ParticleSystemSystem() {
        }

        ParticleSystemSystem::ParticleSystemSystem(const ParticleSystemSystem& orig) {
        }

        ParticleSystemSystem::~ParticleSystemSystem() {
        }
        
        void ParticleSystemSystem::run(EntityManager& em, float deltaTimeSeconds) {
            for(auto itBodies = em.begin({ParticleSystemComponent::getComponentTypeId()}); itBodies != em.end(); ++itBodies) {
                auto& body = itBodies->to<ParticleSystemComponent>().getParticleSystem();
                if(!body.isEnabled()){
                    continue;
                }
                VectorXf grav = VectorXf::Zero(body.getNumParticles()*3);
                for(int i = 0; i<body.getNumParticles(); i++){
                    grav[3*i+1] = 1;
                }
                body.step(deltaTimeSeconds, -0.0981 * grav);
            }
        }
        
        systemId_t ParticleSystemSystem::getSystemTypeId() const {
            return ParticleSystemSystem::systemId;
        }
        
        systemId_t ParticleSystemSystem::systemTypeId() {
            return ParticleSystemSystem::systemId;
        }
        
        void ParticleSystemSystem::setSystemTypeId(systemId_t id) {
            ParticleSystemSystem::systemId = id;
        }
    }
}

