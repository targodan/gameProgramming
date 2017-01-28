/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ParticleSystemComponent.h
 * Author: markus
 *
 * Created on 26. Januar 2017, 14:44
 */

#ifndef PARTICLESYSTEMCOMPONENT_H
#define PARTICLESYSTEMCOMPONENT_H

#include "../ECS/Component.h"
#include "../physics/ParticleSystem.h"
namespace engine {
    namespace ECSCommon {
        using namespace engine::ECS;

        class ParticleSystemComponent : public Component {
        public:
            ParticleSystemComponent() {}
            ParticleSystemComponent(const std::shared_ptr<engine::physics::ParticleSystem>& system) : particleSystem(system){}
            ParticleSystemComponent(const ParticleSystemComponent& orig) = delete;
            virtual ~ParticleSystemComponent(){}

            const engine::physics::ParticleSystem& getParticleSystem() const {
                return *this->particleSystem;
            }

            engine::physics::ParticleSystem& getParticleSystem() {
                return *this->particleSystem;
            }

            componentId_t getComponentId() const override;
            std::string getComponentName() const override;
            std::string toString() const override;

            static void setComponentTypeId(componentId_t id);
            static componentId_t getComponentTypeId();

        private:
            static componentId_t typeId;
        protected:
            std::shared_ptr<engine::physics::ParticleSystem> particleSystem;
        };

    }
}

#endif /* PARTICLESYSTEMCOMPONENT_H */

