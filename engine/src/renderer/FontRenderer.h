#ifndef FONTRENDERER_H
#define FONTRENDERER_H

#include <memory>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "ShaderProgram.h"
#include "RichText.h"

// This implementation is based on https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Text_Rendering_01

namespace engine {
    namespace renderer {
        class Font;
        
        class FontRenderer {
        protected:
            FT_Library ft;
            ShaderProgram shader;
            int windowWidth;
            int windowHeight;
            
            FontRenderer();
            
            void renderChar(char32_t c, const Font& font, const Color& color, float& x, float& y, float scaleX, float scaleY) const;
            
        public:
            void setWindowDimensions(int width, int height);
            void renderRichText(const RichText& text, int xPixel, int yPixel) const;
            void renderText(const std::string& text, const Font& font, const Color& color, int xPixel, int yPixel) const;
            void renderText(const std::u32string& text, const Font& font, const Color& color, int xPixel, int yPixel) const;
            FT_Library& getFT();
            
        protected:
            static std::unique_ptr<FontRenderer> instance;
            
        public:
            static FontRenderer& getInstance();
        };
    }
}

#endif /* FONTRENDERER_H */

