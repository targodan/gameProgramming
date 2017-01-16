#include "Font.h"

#include "../IOException.h"

#include "FontRenderer.h"

namespace engine {
    namespace renderer {
        Font::Font(const char* filename) {
            if(FT_New_Face(FontRenderer::getInstance().getFT(), filename, 0, &this->face)) {
                throw engine::IOException("Could not load font in file \"%s\".", filename);
            }
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
        
        const FT_GlyphSlot& Font::renderChar(wchar_t c) const {
            // TODO: c should be in UTF-32.
            int glyphIndex = FT_Get_Char_Index(this->face, c);
            FT_Load_Glyph(this->face, glyphIndex, FT_LOAD_DEFAULT);
            FT_Render_Glyph(this->face->glyph, FT_RENDER_MODE_NORMAL);
            return this->face->glyph;
        }
    }
}
