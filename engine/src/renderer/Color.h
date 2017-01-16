#ifndef COLOR_H
#define COLOR_H

#include "../util/vec4.h"

namespace engine {
    namespace renderer {
        using namespace engine::util;
        
        class Color {
        protected:
            vec4 color;
            
        public:
            Color(float red, float green, float blue, float alpha = 1.0f) : color(red, green, blue, alpha) {}
            
            bool operator==(const Color& other) const {
                return this->color == other.color;
            }
            bool operator!=(const Color& other) const {
                return this->color != other.color;
            }
            
            float getRed() const {
                return this->color[0];
            }
            float getGreen() const {
                return this->color[1];
            }
            float getBlue() const {
                return this->color[2];
            }
            float getAlpha() const {
                return this->color[3];
            }
            
            Color& setRed(float red) {
                this->color[0] = red;
                return *this;
            }
            Color& setGreen(float green) {
                this->color[1] = green;
                return *this;
            }
            Color& setBlue(float blue) {
                this->color[2] = blue;
                return *this;
            }
            Color& setAlpha(float alpha) {
                this->color[3] = alpha;
                return *this;
            }
            
            const vec4& getGLColor() const {
               return this->color; 
            }
            vec4& getGLColor() {
               return this->color; 
            }
            
            static const Color BLACK;
            static const Color WHITE;
            static const Color RED;
            static const Color GREEN;
            static const Color BLUE;
        };
    }
}

#endif /* COLOR_H */

