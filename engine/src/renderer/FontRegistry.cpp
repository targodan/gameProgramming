#include "FontRegistry.h"

#include "../IllegalArgumentException.h"

namespace engine {
    namespace renderer {
        std::unique_ptr<FontRegistry> FontRegistry::instance = nullptr;

        FontRegistry& FontRegistry::getInstance() {
            if(FontRegistry::instance == nullptr) {
                FontRegistry::instance = std::make_unique<FontRegistry>();
            }
            return *FontRegistry::instance;
        }

        FontFamily& FontRegistry::getFontFamily(const std::string& name) {
            auto it = FontRegistry::getInstance().families.find(name);
            if(it == FontRegistry::getInstance().families.end()) {
                throw IllegalArgumentException("FontFamily of name \"%s\" has not been registered yet.", name.c_str());
            }
            return *it->second;
        }
    }
}
