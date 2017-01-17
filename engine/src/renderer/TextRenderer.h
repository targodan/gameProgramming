#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

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
        
        class TextRenderer {
        protected:
            FT_Library ft;
            
            ShaderProgram shader;
            GLuint vbo;
            GLuint attribute_coord;
            GLuint tex;
            
            bool batchMode = false;
            
            int windowWidth;
            int windowHeight;
            
            struct textPosition {
                float x;
                float y;
                float scaleX;
                float scaleY;
            };
            
            TextRenderer();
            
            void preTextRender();
            void postTextRender();
            textPosition calculateTextPosition(int xPixel, int yPixel) const;
            
            void renderChar(char32_t c, const Font& font, const Color& color, float& x, float& y, float scaleX, float scaleY);
            
        public:
            void setWindowDimensions(int width, int height);
            
            void enableBatchMode();
            void endBatchMode();
            
            void renderRichText(RichText& text, int xPixel, int yPixel);
            void renderText(const std::string& text, const Font& font, const Color& color, int xPixel, int yPixel);
            void renderText(const std::u32string& text, const Font& font, const Color& color, int xPixel, int yPixel);
            
            FT_Library& getFT();
            
        protected:
            static std::unique_ptr<TextRenderer> instance;
            
        public:
            static TextRenderer& getInstance();
        };
    }
}

#endif /* TEXTRENDERER_H */

