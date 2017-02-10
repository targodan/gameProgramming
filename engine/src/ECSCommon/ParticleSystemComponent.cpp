/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ParticleSystemComponent.cpp
 * Author: markus
 * 
 * Created on 26. Januar 2017, 14:45
 */

#include "ParticleSystemComponent.h"

#include "../ECS/ComponentRegisterer.h"
namespace engine {
    namespace ECSCommon {
        ECS_REGISTER_COMPONENT(ParticleSystemComponent);
        
        componentId_t ParticleSystemComponent::typeId = 0;
        
        componentId_t ParticleSystemComponent::getComponentId() const {
            return ParticleSystemComponent::typeId;
        }
        
        std::string ParticleSystemComponent::getComponentName() const {
            return "ParticleSystemComponent";
        }
        
        std::string ParticleSystemComponent::toString() const {
            return "... not gonna try to stuff this into a string.";
        }
        
        void ParticleSystemComponent::setComponentTypeId(componentId_t id) {
            ParticleSystemComponent::typeId = id;
        }
        
        componentId_t ParticleSystemComponent::getComponentTypeId()  {
            return ParticleSystemComponent::typeId;
        }
    }
}


