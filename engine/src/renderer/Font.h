#ifndef FONT_H
#define FONT_H

#include <ft2build.h>
#include FT_FREETYPE_H

// This implementation is based on https://www.freetype.org/freetype2/docs/tutorial/step1.html

namespace engine {
    namespace renderer {
        class Font {
        protected:
            FT_Face face;
            
        public:
            Font(const char* filename);
            
            void setSizeInPixels(int pixel);
            void setSizeInPoint(float pt, int screenDPIHorizontal, int screenDPIVertical = -1);
            
            const FT_GlyphSlot& renderChar(wchar_t c) const;
        };
    }
}

#endif /* FONT_H */

