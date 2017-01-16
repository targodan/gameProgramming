#ifndef FONTRENDERER_H
#define FONTRENDERER_H

#include <memory>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "ShaderProgram.h"

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
            
        public:
            void setWindowDimensions(int width, int height);
            void renderText(const std::wstring& text, const Font& font, int xPixel, int yPixel) const;
            FT_Library& getFT();
            
        protected:
            static std::unique_ptr<FontRenderer> instance;
            
        public:
            static FontRenderer& getInstance();
        };
    }
}

#endif /* FONTRENDERER_H */

