#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "VertexAttribute.h"
#include "Buffer.h"
#include "gl/gl_core_3_3.h"
#include "../util/vector.h"
#include <list>

#include <easylogging++.h>

namespace engine {
    namespace renderer {
        using namespace gl;
        using util::vector;
        
        class VertexBuffer : public Buffer {
        public:
            VertexBuffer() 
                : type(BufferType::ARRAY_BUFFER) {}
            VertexBuffer(const void* dataPtr, size_t size, size_t nElements, DataUsagePattern usage) 
                : Buffer(dataPtr, size, nElements, usage), type(BufferType::ARRAY_BUFFER) {}
            VertexBuffer(const VertexBuffer& orig) 
                : Buffer(orig), type(orig.type), attributes(orig.attributes) {}
            VertexBuffer(VertexBuffer&& orig) 
                : Buffer(std::move(orig)), type(std::move(orig.type)), attributes(std::move(orig.attributes)) {}
            
            VertexBuffer& operator=(const VertexBuffer& right) {
                Buffer::operator=(right);
                
                this->attributes = right.attributes;
                
                return *this;
            }
            VertexBuffer& operator=(VertexBuffer&& right){
                Buffer::operator=(std::move(right));
                
                this->attributes = std::move(right.attributes);
                
                return *this;
            }
            
            virtual ~VertexBuffer() {}

            void setAttributes(const vector<VertexAttribute>& attribs) {
                this->attributes = attribs;
            }
            const vector<VertexAttribute>& getAttributes() const {
                return this->attributes;
            }
            
            void setAttributePointers() {
                for(auto& attribute : this->attributes) {
                    setAttributePointer(attribute);
                    enableAttribute(attribute.index);
                }
            }
            void setAttributePointer(const VertexAttribute& attribute) {
                glVertexAttribPointer(attribute.index, attribute.size, attribute.type, 
                    attribute.normalized, attribute.stride, attribute.offset);
            }
            
            virtual void bind() override {
                if(VertexBuffer::anyVBOBound) {
                    VertexBuffer::anyVBOBound = false;
                    LOG(WARNING) << "Trying to bind a VBO, but there is one bound already.";
                }
            
                Buffer::bind();
                
                VertexBuffer::anyVBOBound = true;
            }
            virtual void unbind() override {
                Buffer::unbind();
                
                VertexBuffer::anyVBOBound = false;
            }
            virtual const BufferType getType() const override {
                return this->type;
            }
            
            static bool isAnyVBOBound() {
                return anyVBOBound;
            }
        private:
            void enableAttribute(GLuint index) {
                glEnableVertexAttribArray(index);
            }
            void disableAttribute(GLuint index) {
                glDisableVertexAttribArray(index);
            }
            
            const BufferType type;
            vector<VertexAttribute> attributes;
            
            static bool anyVBOBound;
        };
    }
}


#endif /* VERTEXBUFFER_H */

