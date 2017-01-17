#ifndef ELEMENTBUFFER_H
#define ELEMENTBUFFER_H

#include "Buffer.h"
#include "gl/gl_core_3_3.h"

namespace engine {
    namespace renderer {
        using namespace gl;
        
        class ElementBuffer : public Buffer {
        public:
            ElementBuffer() 
                : type(BufferType::ELEMENT_ARRAY_BUFFER) {}
            ElementBuffer(const void* dataPtr, size_t size, size_t nElements, DataUsagePattern usage) 
                : Buffer(dataPtr, size, nElements, usage), type(BufferType::ELEMENT_ARRAY_BUFFER) {}
            ElementBuffer(const ElementBuffer& orig) 
                : Buffer(orig), type(orig.type) {}
            ElementBuffer(ElementBuffer&& orig) 
                : Buffer(std::move(orig)), type(std::move(orig.type)) {}
            
            ElementBuffer& operator=(const ElementBuffer& right) {
                Buffer::operator=(right);
                
                return *this;
            }
            ElementBuffer& operator=(ElementBuffer&& right){
                Buffer::operator=(std::move(right));
                
                return *this;
            }
            
            virtual ~ElementBuffer() {}
            
            virtual void bind() override {
                if(ElementBuffer::anyEBOBound) {
                    // TODO: Log warning; maybe bindBuffer to 0?
                    ElementBuffer::anyEBOBound = false;
                }
            
                Buffer::bind();
                
                ElementBuffer::anyEBOBound = true;
            }
            virtual void unbind() override {
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

