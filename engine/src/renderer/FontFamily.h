#ifndef FONTFAMILY_H
#define FONTFAMILY_H

#include <memory>
#include <string>
#include "../util/Map.h"

#include "Font.h"

namespace engine {
    namespace renderer {
        enum class FontType {
            Regular,
            Bold,
            Italic,
            BoldItalic,
            INVALID
        };
        
        class FontFamily {
        protected:
            engine::util::Map<FontType, std::shared_ptr<Font>> fonts;
            
            void loadFont(const std::string& filename, FontType type);
        public:
            FontFamily(const std::string& regularFontFilename);
            FontFamily(const std::string& regularFontFilename, const std::string& boldFontFilename);
            FontFamily(const std::string& regularFontFilename, const std::string& boldFontFilename, const std::string& italicFontFilename);
            FontFamily(const std::string& regularFontFilename, const std::string& boldFontFilename, const std::string& italicFontFilename, const std::string& boldItalicFontFilename);
            
            bool operator==(const FontFamily& other) const;
            bool operator!=(const FontFamily& other) const;
            
            void loadBoldFont(const std::string& filename);
            void loadItalicFont(const std::string& filename);
            void loadBoldItalicFont(const std::string& filename);
            
            Font& getFont(FontType type);
            Font& getRegularFont();
            Font& getBoldFont();
            Font& getItalicFont();
            Font& getBoldItalicFont();
        };
    }
}

#endif /* FONTFAMILY_H */

