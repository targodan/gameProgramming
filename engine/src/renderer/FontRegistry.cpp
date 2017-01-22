#include "FontRegistry.h"

#include "../IllegalArgumentException.h"

namespace engine {
    namespace renderer {
        std::unique_ptr<FontRegistry> FontRegistry::instance = nullptr;

        FontRegistry& FontRegistry::getInstance() {
            if(FontRegistry::instance == nullptr) {
                FontRegistry::instance = std::unique_ptr<FontRegistry>(new FontRegistry());
            }
            return *FontRegistry::instance;
        }
        
        std::shared_ptr<FontFamily>& FontRegistry::getFontFamilyPointer(const std::string& name) {
            auto it = FontRegistry::getInstance().families.find(name);
#ifdef DEBUG
            if(it == FontRegistry::getInstance().families.end()) {
                throw IllegalArgumentException("FontFamily of name \"%s\" has not been registered yet.", name.c_str());
            }
#endif
            return it->second;
        }

        FontFamily& FontRegistry::getFontFamily(const std::string& name) {
            return *FontRegistry::getInstance().getFontFamilyPointer(name);
        }
        
        void FontRegistry::setDefaultFontFamily(const std::string& name) {
            auto& reg = FontRegistry::getInstance();
            reg.defaultFamily = reg.getFontFamilyPointer(name);
        }
        
        FontFamily& FontRegistry::getDefaultFontFamily() {
            auto& reg = FontRegistry::getInstance();
#ifdef DEBUG
            if(reg.defaultFamily == nullptr) {
                throw IllegalArgumentException("Requesting default font, but no font registered yet!");
            }
#endif
            return *reg.defaultFamily;
        }
    }
}
