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

        std::u32string RichText::getPlainText() const {
            std::u32string text;
            for(auto& fragment : this->textFragments) {
                text += fragment.getText();
            }
            return text;
        }
        std::string RichText::getPlainText_utf8() const {
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
        
        const RichTextFragment& RichText::getFragment(size_t i) const {
            return this->textFragments[i];
        }
        RichTextFragment& RichText::getFragment(size_t i) {
            return this->textFragments[i];
        }
        size_t RichText::numberOfFragments() const {
            return this->textFragments.size();
        }
        size_t RichText::length() const {
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
        
        RichText::ConstIterator& RichText::ConstIterator::operator++() {
            if(this->index < this->text->numberOfFragments()) {
                ++this->index;
            }
            return *this;
        }
        RichText::ConstIterator RichText::ConstIterator::operator++(int) {
            ConstIterator copy(*this);
            this->operator++();
            return copy;
        }
        bool RichText::ConstIterator::operator==(RichText::ConstIterator& other) const {
            return this->text == other.text && this->index == other.index;
        }
        bool RichText::ConstIterator::operator!=(RichText::ConstIterator& other) const {
            return !(this->operator==(other));
        }
        const RichTextFragment& RichText::ConstIterator::operator*() const {
            return this->text->getFragment(this->index);
        }
        const RichTextFragment* RichText::ConstIterator::operator->() const {
            return &this->text->getFragment(this->index);
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
        
        RichText::ConstIterator RichText::begin() const {
            return ConstIterator(this, 0);
        }
        RichText::ConstIterator RichText::end() const {
            return ConstIterator(this, this->textFragments.size());
        }
        RichText::Iterator RichText::begin() {
            return Iterator(this, 0);
        }
        RichText::Iterator RichText::end() {
            return Iterator(this, this->textFragments.size());
        }
    }
}
