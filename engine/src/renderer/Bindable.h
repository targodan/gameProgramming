#ifndef BINDABLE_H
#define BINDABLE_H

#include "gl/gl_core_3_3.h"
#include "BufferType.h"
#include "TextureDimension.h"

namespace engine {
    namespace renderer {
        using namespace gl;
        
        class Bindable {
        public:
            Bindable() {};
            Bindable(const Bindable& orig) {};
            Bindable(Bindable&& orig) {};
            virtual ~Bindable() {};
            
            virtual void bind() = 0;
            virtual void unbind() = 0;
            
            virtual bool isBound() const = 0;
             
            static void bind(BufferType type, GLuint id) {
                glBindBuffer(type, id);
            }
            static void unbind(BufferType type) {
                glBindBuffer(type, 0);
            }
            
            static void bindVertexArray(GLuint id) {
                glBindVertexArray(id);
            }
            static void unbindVertexArray() {
                glBindVertexArray(0);
            }
            
            static void bindTexture(TextureDimension type, GLuint id) {
                glBindTexture(type, id);
            }
            static void unbindTexture(TextureDimension type) {
                glBindTexture(type, 0);
            }
        };
    }
}

#endif /* BINDABLE_H */

