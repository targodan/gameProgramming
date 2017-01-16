#include "TextComponent.h"

#include "../ECS/ComponentRegisterer.h"

namespace engine {
    namespace ECSCommon {
        ECS_REGISTER_COMPONENT(TextComponent);
        
        componentId_t TextComponent::typeId = 0;
        
        componentId_t TextComponent::getComponentId() const {
            return TextComponent::typeId;
        }
        
        std::string TextComponent::getComponentName() const {
            return "TextComponent";
        }
        
        std::string TextComponent::toString() const {
            return this->text.getPlainText_utf8();
        }

        void TextComponent::setComponentTypeId(componentId_t id) {
            TextComponent::typeId = id;
        }
        
        componentId_t TextComponent::getComponentTypeId() {
            return TextComponent::typeId;
        }
    }
}
