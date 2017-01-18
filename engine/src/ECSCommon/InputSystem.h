/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   InputSystem.h
 * Author: markus
 *
 * Created on 18. Januar 2017, 12:47
 */

#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include "../ECS/System.h"
#include "../util/Array.h"
#include "../ECS/SystemRegisterer.h"
#include "../IO/ButtonMapping.h"

namespace engine {
    namespace ECSCommon {
        using namespace engine::ECS;
        using engine::util::Array;
        using engine::IO::ButtonMapping;
        
        class InputSystem : public System {
        public:
            InputSystem(const ButtonMapping& bm);
            InputSystem(const InputSystem& orig) = delete;
            virtual ~InputSystem();
            
            void run(EntityManager& em, float deltaTimeSeconds) override;
            
            bool isUpdateSystem() const override {
                return true;
            }
            bool isRenderSystem() const override {
                return false;
            }
            
            void setMap(ButtonMapping newMap) {
                this->map = newMap;
            }
            
            Array<systemId_t> getOptionalDependencies() const override;
            
            systemId_t getSystemTypeId() const override;
            
            std::string getSystemName() const override {
                return "InputSystem";
            }
            
            static systemId_t systemTypeId();
            static void setSystemTypeId(systemId_t id);
        private:
            ButtonMapping map;
            static systemId_t systemId;
        };

    }
}
#endif /* INPUTSYSTEM_H */

