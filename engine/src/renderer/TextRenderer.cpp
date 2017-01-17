#include <freetype2/freetype/ftimage.h>
#include <codecvt>

#include "TextRenderer.h"

#include "../WTFException.h"
#include "gl/gl_core_3_3.h"
#include "Font.h"

#include "../util/unicode.h"
#include "../InvalidStateException.h"
#include "WindowResizeMessage.h"

#include <easylogging++.h>

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
        TextRenderer::TextRenderer() : shader(ShaderProgram::createShaderProgramFromSource(vertexShader, fragmentShader)) {
            if(FT_Init_FreeType(&this->ft)) {
                throw engine::WTFException("Could not initialize freetype library.");
            }
            
            this->attribute_coord = this->shader.getAttributeLocation("coord");
            glGenBuffers(1, &this->vbo);
        }
        
        void TextRenderer::setWindowDimensions(int width, int height) {
            this->windowWidth = width;
            this->windowHeight = height;
        }
        
        void TextRenderer::renderChar(char32_t c, const Font& font, const Color& color, float& x, float& y, float scaleX, float scaleY) {
            auto& glyph = font.renderChar(c);
            
            this->shader.setUniform("color", color.getGLColor());
            
            // TODO: Overlapping glyphs don't alpha-blend but are cut off.
                
            glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RED,
                    glyph->bitmap.width,
                    glyph->bitmap.rows,
                    0,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    glyph->bitmap.buffer
                );

            float x2 = x - glyph->bitmap_left * scaleX;
            float y2 = y + glyph->bitmap_top * scaleY;
            float w = glyph->bitmap.width * scaleX;
            float h = glyph->bitmap.rows * scaleY;

            GLfloat box[4][4] = {
                {x2,     y2    , 0, 0},
                {x2,     y2 - h, 0, 1},
                {x2 + w, y2    , 1, 0},
                {x2 + w, y2 - h, 1, 1},
            };

            glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            
            x += (glyph->advance.x/64) * scaleX;
            y += (glyph->advance.y/64) * scaleY;
        }
        
        void TextRenderer::renderText(const std::string& text, const Font& font, const Color& color, int xPixel, int yPixel) {
            this->renderText(unicode(text), font, color, xPixel, yPixel);
        }
        
        void TextRenderer::preTextRender() {
            this->shader.useProgram();
            
            // TODO: @Tim is this necessary? Is this on anyway? Do I have to reset this afterwards?
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            
            this->tex = 0;
            glActiveTexture(GL_TEXTURE0);
            glGenTextures(1, &this->tex);
            glBindTexture(GL_TEXTURE_2D, this->tex);
            glUniform1i(this->shader.getUniformLocation("tex"), 0);
            
            // Necessary because of the way FreeType renders glyphs
            // TODO: Do I need to reset this after?
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            
            // Prevents artifacts when not exactly on pixel boundaries
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            
            GLuint vao;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);
            
            glEnableVertexAttribArray(this->attribute_coord);
            glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
            glVertexAttribPointer(this->attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);
        }
        
        void TextRenderer::postTextRender() {
            glDisableVertexAttribArray(this->attribute_coord);
            glDeleteTextures(1, &this->tex);
        }
            
        void TextRenderer::enableBatchMode() {
            if(this->batchMode) {
                throw InvalidStateException("Tried to enable batch mode while already in batch mode.");
            }
            this->preTextRender();
            this->batchMode = true;
        }
        
        void TextRenderer::endBatchMode() {
            if(!this->batchMode) {
                throw InvalidStateException("Tried to end batch mode while not yet in batch mode.");
            }
            this->postTextRender();
            this->batchMode = false;
        }
        
        TextRenderer::textPosition TextRenderer::calculateTextPosition(int xPixel, int yPixel) const {
            textPosition pos;
            pos.scaleX = 2.0 / this->windowWidth;
            pos.scaleY = 2.0 / this->windowWidth;
            pos.x = -1 + xPixel * pos.scaleX;
            pos.y = 1 - yPixel * pos.scaleY;
            return pos;
        }
        
        void TextRenderer::renderText(const std::u32string& text, const Font& font, const Color& color, int xPixel, int yPixel) {
            if(!this->batchMode) {
                this->preTextRender();
            }
            
            auto pos = this->calculateTextPosition(xPixel, yPixel);
            
            for(auto& c : text) {
                this->renderChar(c, font, color, pos.x, pos.y, pos.scaleX, pos.scaleY);
            }
            
            if(!this->batchMode) {
                this->postTextRender();
            }
        }
        
        void TextRenderer::renderRichText(RichText& text, int xPixel, int yPixel) {
            if(!this->batchMode) {
                this->preTextRender();
            }
            
            auto pos = this->calculateTextPosition(xPixel, yPixel);
            
            for(auto& fragment : text) {
                for(auto& c : fragment.getText()) {
                    this->renderChar(c, fragment.getSizedFont(), fragment.getColor(), pos.x, pos.y, pos.scaleX, pos.scaleY);
                }
            }
            
            if(!this->batchMode) {
                this->postTextRender();
            }
        }
        
        FT_Library& TextRenderer::getFT() {
            return this->ft;
        }

        void TextRenderer::receive(shared_ptr<engine::ECS::Message> msg) {
            auto& resize = msg->to<WindowResizeMessage>();
            this->windowWidth = resize.getNewWidth();
            this->windowHeight = resize.getNewHeight();
            LOG(INFO) << "Weeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee";
        }
        
        std::unique_ptr<TextRenderer> TextRenderer::instance = nullptr;
        
        TextRenderer* TextRenderer::getInstancePointer() {
            if(TextRenderer::instance == nullptr) {
                TextRenderer::instance = std::unique_ptr<TextRenderer>(new TextRenderer());
            }
            return TextRenderer::instance.get();
        }
        TextRenderer& TextRenderer::getInstance() {
            return *TextRenderer::getInstancePointer();
        }
        
        void TextRenderer::registerForResizeMessages(engine::ECS::MessageHandler& mh) {
            mh.registerReceiver(mh.lookupMessageId(WINDOW_RESIZE_MESSAGE_NAME), TextRenderer::getInstancePointer());
        }
    }
}
