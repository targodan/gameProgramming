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
            
            engine::util::Map<std::string, std::shared_ptr<FontFamily>> families;
            static FontRegistry& getInstance();
            
        public:
            static FontFamily& getFontFamily(const std::string& name);
            
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

