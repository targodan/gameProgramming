#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "VertexAttribute.h"
#include "Buffer.h"
#include "gl/gl_core_3_3.h"
#include "../util/vector.h"
#include <list>

namespace engine {
    namespace renderer {
        using namespace gl;
        using util::vector;
        
        class VertexBuffer : public Buffer {
        public:
            VertexBuffer() 
                : type(BufferType::ARRAY_BUFFER) {
                    
            }
            
            VertexBuffer(const void* dataPtr, size_t size, size_t nElements, DataUsagePattern usage) 
                : Buffer(dataPtr, size, nElements, usage), type(BufferType::ARRAY_BUFFER) {
            }
                
            VertexBuffer(const VertexBuffer& orig) 
                : Buffer(orig), type(orig.type), attributes(orig.attributes) {

            }
            
            VertexBuffer(VertexBuffer&& orig) 
                : Buffer(std::move(orig)), type(std::move(orig.type)), attributes(std::move(orig.attributes)) {

            }
            
            ~VertexBuffer() {
            }

            void setAttributes(const vector<VertexAttribute>& attribs) {
                this->attributes = attribs;
            }
            const vector<VertexAttribute>& getAttributes() const {
                return this->attributes;
            }
            
            virtual void bind() override {
                if(this->bound) {
                    return;
                } else if(VertexBuffer::anyVBOBound) {
                    // TODO: Log warning; maybe bindBuffer to 0?
                    VertexBuffer::anyVBOBound = false;
                }
            
                // glBindBuffer(GL_ARRAY_BUFFER, this->id);
                Buffer::bind();
                
                this->bound = true;
                VertexBuffer::anyVBOBound = true;
            }
            virtual void unbind() override {
                if(!this->bound) {
                    return;
                }

                // glBindBuffer(GL_ARRAY_BUFFER, 0);
                Buffer::unbind();
                
                this->bound = false;
                VertexBuffer::anyVBOBound = false;
            }
            
            virtual const BufferType getType() const override {
                return this->type;
            }
            
            static bool isAnyVBOBound() {
                return anyVBOBound;
            }
        private:
            const BufferType type;
            vector<VertexAttribute> attributes;
            
            static bool anyVBOBound;
        };
    }
}


#endif /* VERTEXBUFFER_H */

