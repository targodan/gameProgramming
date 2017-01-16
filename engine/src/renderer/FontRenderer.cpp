#include <freetype2/freetype/ftimage.h>
#include <codecvt>

#include "FontRenderer.h"

#include "../WTFException.h"
#include "gl/gl_core_3_3.h"
#include "Font.h"

#include "../util/unicode.h"

std::string vertexShader = 
        "#version 120\n"
        "\n"
        "attribute vec4 coord;\n"
        "varying vec2 texcoord;\n"
        "\n"
        "void main() {\n"
        "   gl_Position = vec4(coord.xy, 0, 1);\n"
        "   texcoord = coord.zw;\n"
        "}";
std::string fragmentShader = 
        "#version 120\n"
        "\n"
        "varying vec2 texcoord;\n"
        "uniform sampler2D tex;\n"
        "uniform vec4 color;\n"
        "\n"
        "void main() {\n"
        "   gl_FragColor = vec4(1, 1, 1, texture2D(tex, texcoord).r) * color;\n"
        "}";

using namespace engine::renderer::gl;

namespace engine {
    namespace renderer {
        FontRenderer::FontRenderer() : shader(ShaderProgram::createShaderProgramFromSource(vertexShader, fragmentShader)) {
            if(FT_Init_FreeType(&this->ft)) {
                throw engine::WTFException("Could not initialize freetype library.");
            }
            
            this->attribute_coord = this->shader.getAttributeLocation("coord");
            glGenBuffers(1, &this->vbo);
        }
        
        void FontRenderer::setWindowDimensions(int width, int height) {
            this->windowWidth = width;
            this->windowHeight = height;
        }
        
        void FontRenderer::renderChar(char32_t c, const Font& font, const Color& color, float& x, float& y, float scaleX, float scaleY) {
            auto& glyph = font.renderChar(c);
            
            this->shader.setUniform("color", color.getGLColor());
                
            glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_ALPHA,
                    glyph->bitmap.width,
                    glyph->bitmap.rows,
                    0,
                    GL_ALPHA,
                    GL_UNSIGNED_BYTE,
                    glyph->bitmap.buffer
                );

            float x2 = x + glyph->bitmap_left * scaleX;
            float y2 = -y - glyph->bitmap_top * scaleY;
            float w = glyph->bitmap.width * scaleX;
            float h = glyph->bitmap.rows * scaleY;

            GLfloat box[4][4] = {
                {x2,     -y2    , 0, 0},
                {x2 + w, -y2    , 1, 0},
                {x2,     -y2 - h, 0, 1},
                {x2 + w, -y2 - h, 1, 1},
            };

            glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            
            x += (glyph->advance.x/64) * scaleX;
            y += (glyph->advance.y/64) * scaleY;
        }
        
        void FontRenderer::renderText(const std::string& text, const Font& font, const Color& color, int xPixel, int yPixel) {
            this->renderText(unicode(text), font, color, xPixel, yPixel);
        }
        
        void FontRenderer::preTextRender() {
            // TODO: @Tim is this necessary? Is this on anyway? Do I have to reset this afterwards?
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            
            glActiveTexture(GL_TEXTURE0);
            glGenTextures(1, &this->tex);
            glBindTexture(GL_TEXTURE_2D, this->tex);
            this->shader.setUniform("tex", 0);
            
            // Necessary because of the way FreeType renders glyphs
            // TODO: Do I need to reset this after?
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            
            // Prevents artifacts when not exactly on pixel boundaries
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            
            glEnableVertexAttribArray(this->attribute_coord);
            glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
            glVertexAttribPointer(this->attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);
            
            this->shader.useProgram();
        }
        
        void FontRenderer::postTextRender() {
            glDisableVertexAttribArray(this->attribute_coord);
            glDeleteTextures(1, &this->tex);
        }
        
        FontRenderer::textPosition FontRenderer::calculateTextPosition(int xPixel, int yPixel) const {
            textPosition pos;
            pos.scaleX = 2.0 / this->windowWidth;
            pos.scaleY = 2.0 / this->windowWidth;
            pos.x = -1 + xPixel * pos.scaleX;
            pos.y = 1 - yPixel * pos.scaleY;
            return pos;
        }
        
        void FontRenderer::renderText(const std::u32string& text, const Font& font, const Color& color, int xPixel, int yPixel) {
            this->preTextRender();
            
            auto pos = this->calculateTextPosition(xPixel, yPixel);
            
            for(auto& c : text) {
                this->renderChar(c, font, color, pos.x, pos.y, pos.scaleX, pos.scaleY);
            }
            
            this->postTextRender();
        }
        
        void FontRenderer::renderRichText(RichText& text, int xPixel, int yPixel) {
            this->preTextRender();
            
            auto pos = this->calculateTextPosition(xPixel, yPixel);
            
            for(auto& fragment : text) {
                for(auto& c : fragment.getText()) {
                    this->renderChar(c, fragment.getSizedFont(), fragment.getColor(), pos.x, pos.y, pos.scaleX, pos.scaleY);
                }
            }
            
            this->postTextRender();
        }
        
        FT_Library& FontRenderer::getFT() {
            return this->ft;
        }
        
        std::unique_ptr<FontRenderer> FontRenderer::instance = nullptr;
        
        FontRenderer& FontRenderer::getInstance() {
            if(FontRenderer::instance == nullptr) {
                FontRenderer::instance = std::unique_ptr<FontRenderer>(new FontRenderer());
            }
            return *FontRenderer::instance;
        }
    }
}
