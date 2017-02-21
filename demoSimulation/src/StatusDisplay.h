#ifndef STATUSDISPLAY_H
#define STATUSDISPLAY_H

#include "engine/ECSCommon/TextComponent.h"
#include "engine/ECS/EntityManager.h"

namespace demoSimulation {
    using engine::ECSCommon::TextComponent;
    using engine::ECS::EntityManager;
    using engine::ECS::Entity;
    using namespace engine::renderer;
    
    class StatusDisplay {
        int fontSize;
        RichText defaultText;
        Entity gravity;
        Entity leftFrozen;
        Entity rightFrozen;
        Entity topFrozen;
        Entity bottomFrozen;
        
    public:
        StatusDisplay(EntityManager& em) : fontSize(20), defaultText(FontRegistry::getDefaultFontFamily(), FontType::Regular, fontSize, Color::BLUE) {
            int y = 600;
            this->gravity = em.createEntity("StatusDisplay")
                    .addComponent<TextComponent>(this->defaultText, 10, y);
            y += this->fontSize * 1.5;
            this->leftFrozen = em.createEntity("StatusDisplay")
                    .addComponent<TextComponent>(this->defaultText, 10, y);
            y += this->fontSize * 1.5;
            this->rightFrozen = em.createEntity("StatusDisplay")
                    .addComponent<TextComponent>(this->defaultText, 10, y);
            y += this->fontSize * 1.5;
            this->topFrozen = em.createEntity("StatusDisplay")
                    .addComponent<TextComponent>(this->defaultText, 10, y);
            y += this->fontSize * 1.5;
            this->bottomFrozen = em.createEntity("StatusDisplay")
                    .addComponent<TextComponent>(this->defaultText, 10, y);
        }
                
        void setGravity(bool on) {
            RichText text = this->defaultText;
            text << "Gravity (G): " << (on ? "on" : "off");
            
            this->gravity.getComponent<TextComponent>().setText(text);
        }
        
        void setLeftFrozen(bool on) {
            RichText text = this->defaultText;
            text << "Left frozen (H): " << (on ? "yes" : "no");
            
            this->leftFrozen.getComponent<TextComponent>().setText(text);
        }
        
        void setRightFrozen(bool on) {
            RichText text = this->defaultText;
            text << "Right frozen (J): " << (on ? "yes" : "no");
            
            this->rightFrozen.getComponent<TextComponent>().setText(text);
        }
        
        void setTopFrozen(bool on) {
            RichText text = this->defaultText;
            text << "Top frozen (K): " << (on ? "yes" : "no");
            
            this->topFrozen.getComponent<TextComponent>().setText(text);
        }
        
        void setBottomFrozen(bool on) {
            RichText text = this->defaultText;
            text << "Bottom frozen (L): " << (on ? "yes" : "no");
            
            this->bottomFrozen.getComponent<TextComponent>().setText(text);
        }
    };
}

#endif /* STATUSDISPLAY_H */

