#include "Font.h"

#include "../IOException.h"

#include "TextRenderer.h"

namespace engine {
    namespace renderer {
        Font::Font(const std::string& filename) : Font(filename.c_str()) {}
        Font::Font(const char* filename) {
            if(FT_New_Face(TextRenderer::getInstance().getFT(), filename, 0, &this->face)) {
                throw engine::IOException("Could not load font in file \"%s\".", filename);
            }
            if(FT_Select_Charmap(face , ft_encoding_unicode)) {
                throw engine::IOException("Could not load font in file \"%s\". Charmap not supported.", filename);
            }
        }
        
        Font::Font(const Font& orig) : face(orig.face) {}
        Font::Font(Font&& orig) : face(std::move(orig.face)) {}
        Font& Font::operator=(const Font& orig) {
            this->face = orig.face;
            return *this;
        }
        Font& Font::operator=(Font&& orig) {
            std::swap(this->face, orig.face);
            return *this;
        }
            
        bool Font::operator==(const Font& other) const {
            return this->face == other.face;
        }
        bool Font::operator!=(const Font& other) const {
            return !(this->operator==(other));
        }
        
        void Font::setSizeInPixels(int pixel) {
            FT_Set_Pixel_Sizes(this->face, 0, pixel);
        }
        void Font::setSizeInPoint(float pt, int screenDPIHorizontal, int screenDPIVertical) {
            if(screenDPIVertical == -1) {
                screenDPIVertical = screenDPIHorizontal;
            }
            FT_Set_Char_Size(this->face, 0, static_cast<int>(pt*64 + 0.5f), screenDPIHorizontal, screenDPIVertical);
        }
        
        const FT_GlyphSlot& Font::renderChar(char32_t c) const {
            int glyphIndex = FT_Get_Char_Index(this->face, c);
            FT_Load_Glyph(this->face, glyphIndex, FT_LOAD_DEFAULT);
            FT_Render_Glyph(this->face->glyph, FT_RENDER_MODE_NORMAL);
            return this->face->glyph;
        }
    }
}
