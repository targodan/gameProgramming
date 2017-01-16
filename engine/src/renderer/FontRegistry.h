#ifndef FONTREGISTRY_H
#define FONTREGISTRY_H

#include <memory>
#include <string>
#include "../util/Map.h"

#include "FontFamily.h"

namespace engine {
    namespace renderer {
        class FontRegistry {
        protected:
            static std::unique_ptr<FontRegistry> instance;
            static FontRegistry& getInstance();
            
            engine::util::Map<std::string, std::shared_ptr<FontFamily>> families;
            std::shared_ptr<FontFamily> defaultFamily;
            
            std::shared_ptr<FontFamily>& getFontFamilyPointer(const std::string& name);
            
            FontRegistry() {
                this->families.set_empty_key("");
            }
            
        public:
            static FontFamily& getFontFamily(const std::string& name);
            static void setDefaultFontFamily(const std::string& name);
            static FontFamily& getDefaultFontFamily();
            
            template<typename... Args>
            static FontFamily& registerFontFamily(const std::string& name, Args... args) {
                auto& reg = FontRegistry::getInstance();
                reg.families[name] = std::make_shared<FontFamily>(args...);
                return *reg.families[name];
            }
        };
    }
}

#endif /* FONTREGISTRY_H */

