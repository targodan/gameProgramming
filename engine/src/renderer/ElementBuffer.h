#ifndef ELEMENTBUFFER_H
#define ELEMENTBUFFER_H

#include "Buffer.h"
#include "gl/gl_core_3_3.h"

namespace engine {
    namespace renderer {
        using namespace gl;
        
        class ElementBuffer : public Buffer {
        public:
            ElementBuffer() : type(BufferType::ELEMENT_ARRAY_BUFFER) {
            }
            ElementBuffer(const ElementBuffer& orig) 
                : Buffer(orig), type(orig.type) {
            }
            ElementBuffer(ElementBuffer&& orig) 
                : Buffer(std::move(orig)), type(std::move(orig.type)) {
            }
            virtual ~ElementBuffer() {
            }
            
            void generateBuffer() {
                glGenBuffers(1, &(this->id));
            }
            void releaseBuffer() {
                glDeleteBuffers(1, &(this->id));
            }
            
            virtual void bind() override {
                if(this->bound) {
                    return;
                } else if(ElementBuffer::anyEBOBound) {
                    // TODO: Log warning; maybe bindBuffer to 0?
                    ElementBuffer::anyEBOBound = false;
                }
            
                // glBindBuffer(ELEMENT_ARRAY_BUFFER, this->id);
                Buffer::bind();
                ElementBuffer::anyEBOBound = true;
            }
            virtual void unbind() override {
                if(!this->bound) {
                    return;
                }

                // glBindBuffer(ELEMENT_ARRAY_BUFFER, 0);
                Buffer::unbind();
                ElementBuffer::anyEBOBound = false;
            }
            
            virtual const BufferType getType() const override {
                return this->type;
            }
            
            static bool isAnyEBOBound() {
                return anyEBOBound;
            }
        private:
            const BufferType type;
            
            static bool anyEBOBound;
        };
    }
}

#endif /* ELEMENTBUFFER_H */

