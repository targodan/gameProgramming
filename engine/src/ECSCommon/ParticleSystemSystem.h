/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ParticleSystemSystem.h
 * Author: markus
 *
 * Created on 27. Januar 2017, 10:31
 */

#ifndef PARTICLESYSTEMSYSTEM_H
#define PARTICLESYSTEMSYSTEM_H

#include "../ECS/EntityManager.h"
#include "../ECS/System.h"

namespace engine {
    namespace ECSCommon {
        using namespace engine::ECS;

        class ParticleSystemSystem : public System{
        public:
            ParticleSystemSystem();
            ParticleSystemSystem(const ParticleSystemSystem& orig);
            virtual ~ParticleSystemSystem();
            
            
            void run(EntityManager& em, float deltaTimeSeconds) override;
            
            bool isUpdateSystem() const override {
                return true;
            }
            bool isRenderSystem() const override {
                return false;
            }
            
            systemId_t getSystemTypeId() const override;
            
            std::string getSystemName() const override {
                return "ParticleSystemSystem";
            }
            
            static systemId_t systemTypeId();
            static void setSystemTypeId(systemId_t id);
            
        private:
            static systemId_t systemId;

        };
    }
}


#endif /* PARTICLESYSTEMSYSTEM_H */

