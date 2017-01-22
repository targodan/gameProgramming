#ifndef TEXTCOMPONENT_H
#define TEXTCOMPONENT_H

#include "../ECS/Component.h"
#include "../renderer/RichText.h"
#include "../renderer/FontRegistry.h"
#include "../renderer/Color.h"

namespace engine {
    namespace ECSCommon {
        using namespace engine::ECS;
        using namespace engine::renderer;
        
        class TextComponent : public Component {
        private:
            static componentId_t typeId;
            
        protected:
            RichText text;
            int xPixel;
            int yPixel;
            
        public:
            TextComponent() : text(FontRegistry::getDefaultFontFamily(), FontType::Regular, 16, Color::RED) {};
            TextComponent(const RichText& text, int xPixel, int yPixel)
                : text(text), xPixel(xPixel), yPixel(yPixel) {}
            TextComponent(const TextComponent& orig) = delete;
            virtual ~TextComponent() {}
            
            const RichText& getText() const {
                return this->text;
            }
            RichText& getText() {
                return this->text;
            }

            int getXPixel() const {
                return this->xPixel;
            }
            int getYPixel() const {
                return this->yPixel;
            }
            void setXPixel(int x) {
                this->xPixel = x;
            }
            void setYPixel(int y) {
                this->yPixel = y;
            }
            
            componentId_t getComponentId() const override;
            std::string getComponentName() const override;
            std::string toString() const override;

            static void setComponentTypeId(componentId_t id);
            static componentId_t getComponentTypeId();
        };
    }
}

#endif /* TEXTCOMPONENT_H */

