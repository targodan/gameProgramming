#ifndef RICHTEXT_H
#define RICHTEXT_H

#include "FontFamily.h"
#include "Color.h"
#include "../util/unicode.h"
#include "../util/vector.h"

#include <sstream>

namespace engine {
    namespace renderer {
        class RichTextFragment {
        protected:
            Font font;
            Color color;
            int sizeInPixels;
            std::u32string text;
            
        public:
            RichTextFragment(const Font& font, const Color& color, int sizeInPixels, const std::string& text);
            RichTextFragment(const Font& font, const Color& color, int sizeInPixels, const std::u32string& text);
            
            RichTextFragment(const RichTextFragment& orig);
            RichTextFragment(RichTextFragment&& orig);
            
            RichTextFragment& operator=(const RichTextFragment& orig);
            RichTextFragment& operator=(RichTextFragment&& orig);
            
            const Font& getSizedFont();
            const Color& getColor() const;
            const std::u32string& getText() const;
        };
        
        struct RichTextFontFamily {
            FontFamily& fontfamily;
            RichTextFontFamily(FontFamily& fontfamily) : fontfamily(fontfamily) {}
        };
        struct RichTextFontType {
            FontType type;
            RichTextFontType(FontType type) : type(type) {}
        };
        struct RichTextColor {
            Color color;
            RichTextColor(Color color) : color(color) {}
        };
        struct RichTextSize {
            int size;
            RichTextSize(int size) : size(size) {}
        };
        
        class RichText {
            using stringstream = std::basic_stringstream<char32_t>;
        protected:
            FontFamily& defaultFontFamily;
            FontType defaultFontType;
            int defaultSize;
            Color defaultColor;
            
            FontFamily& currentFontFamily;
            FontType currentFontType;
            int currentSize;
            Color currentColor;
            stringstream currentText;
            
            engine::util::vector<RichTextFragment> textFragments;
            
            void flushCurrentText();
            
        public:
            RichText(FontFamily& defaultFontFamily, FontType defaultFontType, int defaultSize, Color defaultColor);
            RichText(const RichText& orig);
            RichText(RichText&& orig);
            
            RichText& operator=(const RichText& orig);
            RichText& operator=(RichText&& orig);
            
            std::u32string getPlainText();
            std::string getPlainText_utf8();
            
            RichText& addText(const std::u32string& text);
            RichText& addText(const std::string& text);
            RichText& addText(FontFamily& fontFamily, FontType fontType, int size, Color color, const std::u32string& text);
            RichText& addText(FontFamily& fontFamily, FontType fontType, int size, Color color, const std::string& text);
            
            class Iterator {
                RichText* text;
                size_t index;
                
            public:
                Iterator(RichText* text, size_t index) : text(text), index(index) {}
                
                Iterator& operator++();
                Iterator operator++(int);
                bool operator==(Iterator& other);
                bool operator!=(Iterator& other);
                RichTextFragment& operator*();
                RichTextFragment* operator->();
            };
            
            RichTextFragment& getFragment(size_t i);
            size_t numberOfFragments();
            size_t length();
            
            Iterator begin();
            Iterator end();
            
            template<typename T>
            RichText& operator<<(T element) {
                this->currentText << element;
                return *this;
            }
            RichText& operator<<(const std::string& text);
            RichText& operator<<(const char* text);
            
            RichText& operator<<(const RichTextFontFamily& style);
            RichText& operator<<(const RichTextFontType& style);
            RichText& operator<<(const RichTextColor& style);
            RichText& operator<<(const RichTextSize& style);
            
            static RichTextFontFamily fontFamily(FontFamily& fontfamily);
            static RichTextFontFamily fontFamily(const std::string& familyName);
            static RichTextFontType fontType(FontType fonttype);
            static RichTextColor color(const Color& color);
            static RichTextSize size(int size);
        };
    }
}

#endif /* RICHTEXT_H */

