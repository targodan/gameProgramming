#ifndef COLOR_H
#define COLOR_H

#include "../util/vec3.h"
#include "../util/vec4.h"

namespace engine {
    namespace renderer {
        using namespace engine::util;
        
        class Color {
        protected:
            vec4 color;
            
        public:
            Color() {}
            Color(float red, float green, float blue, float alpha = 1.0f) : color(red, green, blue, alpha) {}
            
            Color& operator=(const vec3& right) {
                this->color = vec4{right.x, right.y, right.z, 1.0f};
                
                return *this;
            }
            Color& operator=(const vec4& right) {
                this->color = right;
                
                return *this;
            }
            
            bool operator==(const Color& other) const {
                return this->color[0] == other.color[0]
                        && this->color[1] == other.color[1]
                        && this->color[2] == other.color[2]
                        && this->color[3] == other.color[3];
            }
            bool operator!=(const Color& other) const {
                return !(this->operator==(other));
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

