#ifndef ELEMENTBUFFER_H
#define ELEMENTBUFFER_H

#include "Buffer.h"
#include "gl/gl_core_3_3.h"

namespace engine {
    namespace renderer {
        using namespace gl;
        
        class ElementBuffer : public Buffer {
        public:
            ElementBuffer(DataUsagePattern usage = DataUsagePattern::STATIC_DRAW) 
                : Buffer(usage), type(BufferType::ELEMENT_ARRAY_BUFFER) {}
            ElementBuffer(const ElementBuffer& orig) = delete;
            ElementBuffer(ElementBuffer&& orig) 
                : Buffer(std::move(orig)), type(std::move(orig.type)) {}
            
            ElementBuffer& operator=(ElementBuffer&& right){
                Buffer::operator=(std::move(right));
                
                return *this;
            }
            
            virtual ~ElementBuffer() {}
            
            virtual void bind() override {
                if(ElementBuffer::anyEBOBound) {
                    LOG(WARNING) << "Trying to bind a EBO, but there is one bound already.";
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

