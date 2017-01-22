#ifndef FONT_H
#define FONT_H

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>

// This implementation is based on https://www.freetype.org/freetype2/docs/tutorial/step1.html

namespace engine {
    namespace renderer {
        class Font {
        protected:
            FT_Face face;
            
        public:
            Font(const std::string& filename);
            Font(const char* filename);
            Font(const Font& orig);
            Font(Font&& orig);
            
            Font& operator=(const Font& orig);
            Font& operator=(Font&& orig);
            
            bool operator==(const Font& other) const;
            bool operator!=(const Font& other) const;
            
            void setSizeInPixels(int pixel);
            void setSizeInPoint(float pt, int screenDPIHorizontal, int screenDPIVertical = -1);
            
            const FT_GlyphSlot& renderChar(char32_t c) const;
        };
    }
}

#endif /* FONT_H */

