#include "RichText.h"
#include "FontRegistry.h"

namespace engine {
    namespace renderer {
        RichTextFragment::RichTextFragment(const Font& font, const Color& color, int sizeInPixels, const std::string& text)
            : RichTextFragment(font, color, sizeInPixels, unicode(text)) {}
        RichTextFragment::RichTextFragment(const Font& font, const Color& color, int sizeInPixels, const std::u32string& text)
            : font(font), color(color), sizeInPixels(sizeInPixels), text(text) {}
            
        RichTextFragment::RichTextFragment(const RichTextFragment& orig)
            : font(orig.font), color(orig.color), sizeInPixels(orig.sizeInPixels), text(orig.text) {}
        RichTextFragment::RichTextFragment(RichTextFragment&& orig)
            : font(std::move(orig.font)), color(std::move(orig.color)), sizeInPixels(std::move(orig.sizeInPixels)), text(std::move(orig.text)) {}

        RichTextFragment& RichTextFragment::operator=(const RichTextFragment& orig) {
            this->font = orig.font;
            this->color = orig.color;
            this->sizeInPixels = orig.sizeInPixels;
            this->text = orig.text;
            return *this;
        }
        RichTextFragment& RichTextFragment::operator=(RichTextFragment&& orig) {
            std::swap(this->font, orig.font);
            std::swap(this->color, orig.color);
            std::swap(this->sizeInPixels, orig.sizeInPixels);
            std::swap(this->text, orig.text);
            return *this;
        }

        const Font& RichTextFragment::getSizedFont() {
            this->font.setSizeInPixels(this->sizeInPixels);
            return this->font;
        }
        const Color& RichTextFragment::getColor() const {
            return this->color;
        }
        const std::u32string& RichTextFragment::getText() const {
            return this->text;
        }
        
        
        RichText::RichText(FontFamily& defaultFontFamily, FontType defaultFontType, int defaultSize, Color defaultColor)
                : defaultFontFamily(defaultFontFamily), defaultFontType(defaultFontType), defaultSize(defaultSize), defaultColor(defaultColor),
                    currentFontFamily(defaultFontFamily), currentFontType(defaultFontType), currentSize(defaultSize), currentColor(defaultColor) {}
        
        RichText::RichText(const RichText& orig)
                : defaultFontFamily(orig.defaultFontFamily), defaultFontType(orig.defaultFontType), defaultSize(orig.defaultSize), defaultColor(orig.defaultColor),
                    currentFontFamily(orig.defaultFontFamily), currentFontType(orig.defaultFontType), currentSize(orig.defaultSize), currentColor(orig.defaultColor),
                    textFragments(orig.textFragments) {
            this->currentText.str(orig.currentText.str());
        }
        RichText::RichText(RichText&& orig)
                : defaultFontFamily(orig.defaultFontFamily), defaultFontType(std::move(orig.defaultFontType)), defaultSize(std::move(orig.defaultSize)), defaultColor(std::move(orig.defaultColor)),
                    currentFontFamily(orig.defaultFontFamily), currentFontType(std::move(orig.defaultFontType)), currentSize(std::move(orig.defaultSize)), currentColor(std::move(orig.defaultColor)),
                    currentText(std::move(orig.currentText)), textFragments(std::move(orig.textFragments)) {}

        RichText& RichText::operator=(const RichText& orig) {
            this->defaultColor = orig.defaultColor;
            this->defaultFontFamily = orig.defaultFontFamily;
            this->defaultFontType = orig.defaultFontType;
            this->defaultSize = orig.defaultSize;
            
            this->currentColor = orig.currentColor;
            this->currentFontFamily = orig.currentFontFamily;
            this->currentFontType = orig.currentFontType;
            this->currentSize = orig.currentSize;
            
            this->currentText.str(orig.currentText.str());
            
            this->textFragments = orig.textFragments;
            
            return *this;
        }
        RichText& RichText::operator=(RichText&& orig) {
            std::swap(this->defaultColor, orig.defaultColor);
            std::swap(this->defaultFontFamily, orig.defaultFontFamily);
            std::swap(this->defaultFontType, orig.defaultFontType);
            std::swap(this->defaultSize, orig.defaultSize);
            
            std::swap(this->currentColor, orig.currentColor);
            std::swap(this->currentFontFamily, orig.currentFontFamily);
            std::swap(this->currentFontType, orig.currentFontType);
            std::swap(this->currentSize, orig.currentSize);
            
            std::swap(this->currentText, orig.currentText);
            
            std::swap(this->textFragments, orig.textFragments);
            
            return *this;
        }

        std::u32string RichText::getPlainText() {
            this->flushCurrentText();
            std::u32string text;
            for(auto& fragment : this->textFragments) {
                text += fragment.getText();
            }
            return text;
        }
        std::string RichText::getPlainText_utf8() {
            return utf8(this->getPlainText());
        }
        
        void RichText::flushCurrentText() {
            auto text = this->currentText.str();
            if(text.length() > 0) {
                this->textFragments.push_back(RichTextFragment(this->currentFontFamily.getFont(this->currentFontType), this->currentColor, this->currentSize, text));
                this->currentText.str(std::u32string());
            }
        }
        
        RichText& RichText::operator<<(const std::string& text) {
            return this->operator<<(unicode(text));
        }
        RichText& RichText::operator<<(const char* text) {
            return this->operator<<(std::string(text));
        }

        RichText& RichText::addText(const std::u32string& text) {
            this->flushCurrentText();
            this->textFragments.push_back(RichTextFragment(this->defaultFontFamily.getFont(this->defaultFontType), this->defaultColor, this->defaultSize, text));
            return *this;
        }
        RichText& RichText::addText(const std::string& text) {
            this->flushCurrentText();
            this->addText(unicode(text));
            return *this;
        }
        RichText& RichText::addText(FontFamily& fontFamily, FontType fontType, int size, Color color, const std::u32string& text) {
            this->flushCurrentText();
            this->textFragments.push_back(RichTextFragment(fontFamily.getFont(fontType), color, size, text));
            return *this;
        }
        RichText& RichText::addText(FontFamily& fontFamily, FontType fontType, int size, Color color, const std::string& text) {
            this->flushCurrentText();
            this->addText(fontFamily, fontType, size, color, unicode(text));
            return *this;
        }
        
        RichTextFragment& RichText::getFragment(size_t i) {
            this->flushCurrentText();
            return this->textFragments[i];
        }
        size_t RichText::numberOfFragments() {
            this->flushCurrentText();
            return this->textFragments.size();
        }
        size_t RichText::length() {
            this->flushCurrentText();
            size_t len = 0;
            for(auto& frag : this->textFragments) {
                len += frag.getText().length();
            }
            return len;
        }
        
        RichText& RichText::operator<<(const RichTextFontFamily& style) {
            if(this->currentFontFamily != style.fontfamily) {
                this->flushCurrentText();
                this->currentFontFamily = style.fontfamily;
            }
            return *this;
        }
        RichText& RichText::operator<<(const RichTextFontType& style) {
            if(this->currentFontType != style.type) {
                this->flushCurrentText();
                this->currentFontType = style.type;
            }
            return *this;
        }
        RichText& RichText::operator<<(const RichTextColor& style) {
            if(this->currentColor != style.color) {
                this->flushCurrentText();
                this->currentColor = style.color;
            }
            return *this;
        }
        RichText& RichText::operator<<(const RichTextSize& style) {
            if(this->currentSize != style.size) {
                this->flushCurrentText();
                this->currentSize = style.size;
            }
            return *this;
        }

        RichTextFontFamily RichText::fontFamily(FontFamily& fontfamily) {
            return RichTextFontFamily(fontfamily);
        }
        RichTextFontFamily RichText::fontFamily(const std::string& familyName) {
            return RichText::fontFamily(FontRegistry::getFontFamily(familyName));
        }
        RichTextFontType RichText::fontType(FontType fonttype) {
            return RichTextFontType(fonttype);
        }
        RichTextColor RichText::color(const Color& color) {
            return RichTextColor(color);
        }
        RichTextSize RichText::size(int size) {
            return RichTextSize(size);
        }
        
        RichText::Iterator& RichText::Iterator::operator++() {
            if(this->index < this->text->numberOfFragments()) {
                ++this->index;
            }
            return *this;
        }
        RichText::Iterator RichText::Iterator::operator++(int) {
            Iterator copy(*this);
            this->operator++();
            return copy;
        }
        bool RichText::Iterator::operator==(RichText::Iterator& other) {
            return this->text == other.text && this->index == other.index;
        }
        bool RichText::Iterator::operator!=(RichText::Iterator& other) {
            return !(this->operator==(other));
        }
        RichTextFragment& RichText::Iterator::operator*() {
            return this->text->getFragment(this->index);
        }
        RichTextFragment* RichText::Iterator::operator->() {
            return &this->text->getFragment(this->index);
        }
        
        RichText::Iterator RichText::begin() {
            this->flushCurrentText();
            return Iterator(this, 0);
        }
        RichText::Iterator RichText::end() {
            this->flushCurrentText();
            return Iterator(this, this->textFragments.size());
        }
    }
}
