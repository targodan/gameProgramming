#include <climits>

#include "FontFamily.h"
#include "WTFException.h"

namespace engine {
    namespace renderer {
        FontFamily::FontFamily(const std::string& regularFontFilename) {
            this->fonts.set_empty_key(FontType::INVALID);
            this->loadFont(regularFontFilename, FontType::Regular);
        }
        FontFamily::FontFamily(const std::string& regularFontFilename, const std::string& boldFontFilename)
            : FontFamily(regularFontFilename) {
            this->loadFont(boldFontFilename, FontType::Bold);
        }
        FontFamily::FontFamily(const std::string& regularFontFilename, const std::string& boldFontFilename, const std::string& italicFontFilename)
            : FontFamily(regularFontFilename, boldFontFilename) {
            this->loadFont(italicFontFilename, FontType::Italic);
        }
        FontFamily::FontFamily(const std::string& regularFontFilename, const std::string& boldFontFilename, const std::string& italicFontFilename, const std::string& boldItalicFontFilename)
            : FontFamily(regularFontFilename, boldFontFilename, italicFontFilename) {
            this->loadFont(boldItalicFontFilename, FontType::BoldItalic);
        }
        
        bool FontFamily::operator==(const FontFamily& other) const {
            if(this->fonts.size() != other.fonts.size()) {
                return false;
            }
            for(auto& pair : this->fonts) {
                auto otherIt = other.fonts.find(pair.first);
                if(otherIt == other.fonts.end()) {
                    return false;
                } else {
                    if(*pair.second != *otherIt->second) {
                        return false;
                    }
                }
            }
            return true;
        }
        bool FontFamily::operator!=(const FontFamily& other) const {
            return !(this->operator==(other));
        }

        void FontFamily::loadBoldFont(const std::string& filename) {
            this->loadFont(filename, FontType::Bold);
        }
        void FontFamily::loadItalicFont(const std::string& filename) {
            this->loadFont(filename, FontType::Italic);
        }
        void FontFamily::loadBoldItalicFont(const std::string& filename) {
            this->loadFont(filename, FontType::BoldItalic);
        }
        
        Font& FontFamily::getRegularFont() {
            return this->getFont(FontType::Regular);
        }
        Font& FontFamily::getBoldFont() {
            return this->getFont(FontType::Bold);
        }
        Font& FontFamily::getItalicFont() {
            return this->getFont(FontType::Italic);
        }
        Font& FontFamily::getBoldItalicFont() {
            return this->getFont(FontType::BoldItalic);
        }
        
        void FontFamily::loadFont(const std::string& filename, FontType type) {
            this->fonts[type] = std::make_shared<Font>(filename);
        }
            
        Font& FontFamily::getFont(FontType type) {
            auto it = this->fonts.find(type);
            if(it != this->fonts.end()) {
                return *it->second;
            }
            switch(type) {
            case FontType::Regular:
                // Couldn't find regular font... wtf?
                throw WTFException("How were you able to create a FontFamily without a regular font?!");
                
            case FontType::Italic:
                return this->getFont(FontType::Regular);
                
            case FontType::Bold:
                return this->getFont(FontType::Italic);
                
            case FontType::BoldItalic:
                return this->getFont(FontType::Bold);
                
            default:
                // Invalid FontType...
                throw WTFException("You requested an invalid FontType?!");
            }
        }
    }
}
