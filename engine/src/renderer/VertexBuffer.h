#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "VertexAttribute.h"
#include "DataUsagePattern.h"
#include "gl/gl_core_3_3.h"
#include "WTFException.h"
#include "util/vector.h"
#include <list>

namespace engine {
    namespace renderer {
        using namespace gl;
        using util::vector;
        
        struct BufferData {
            const void* dataPtr;
            size_t size;
            DataUsagePattern usage;
        };
        
        /*
         * VertexBuffer is a simple wrapper for VBOs. It does NOT hold the data
         * to render, but rather handles it -> it sets up the space on the 
         * graphics card, saves how the graphics card should interpret the data,
         * can load the data to it and finally release the space again.
         */
        class VertexBuffer {
        public:
            VertexBuffer() 
                : data {nullptr, 0, DataUsagePattern::STATIC_DRAW}, bound(false) {
                this->generateBuffer();
            }
            
            VertexBuffer(const void* dataPtr, size_t size, DataUsagePattern usage) 
                : data {dataPtr, size, usage}, bound(false) {
                this->generateBuffer();
            }
                
            VertexBuffer(const VertexBuffer& orig) 
                : id(orig.id), data(orig.data), attributes(orig.attributes), bound(orig.bound) {
                // Note: the copy points to the exact same buffer on the graphics card
            }
            
            VertexBuffer(VertexBuffer&& orig) 
                : id(std::move(orig.id)), data(std::move(orig.data)), 
                  attributes(std::move(orig.attributes)), bound(std::move(orig.bound)) {
                // Note: I don't know if this is necessary. When this function returns,
                //       will orig be deleted? If so, its deletion would release its
                //       buffer. Therefore, a new buffer has to be generated.
                
                orig.releaseBuffer();
                this->generateBuffer();
            }
            
            ~VertexBuffer() {
                this->releaseBuffer();
            }
            
            void generateBuffer() {
                glGenBuffers(1, &(this->id));
            }
            void releaseBuffer() {
                glDeleteBuffers(1, &(this->id));
            }

            void setAttributes(const std::list<VertexAttribute> attribs) {
                this->attributes = attribs;
            }
            const std::list<VertexAttribute>& getAttributes() const {
                return this->attributes;
            }
            
            void loadData() {
                if(!this->data.dataPtr) {
                    throw WTFException("Could not buffer data: No data specified");
                }
                
                glBufferData(GL_ARRAY_BUFFER, data.size, data.dataPtr, data.usage);
            }
            void loadData(const void* data, size_t size, DataUsagePattern usage) {
                glBufferData(GL_ARRAY_BUFFER, size, data, usage);
                this->data = {data, size, usage};
            }
            
            
            GLuint getID() const {
                return this->id;
            }
            
            void bind() const {
                if(this->bound) {
                    return;
                } else if(VertexBuffer::anyVBOBound) {
                    // TODO: Log warning; maybe bindBuffer to 0?
                    VertexBuffer::anyVBOBound = false;
                }
            
                glBindBuffer(GL_ARRAY_BUFFER, this->id);
                VertexBuffer::anyVBOBound = true;
            }
            void unbind() const {
                if(!this->bound) {
                    return;
                }

                glBindBuffer(GL_ARRAY_BUFFER, 0);
                VertexBuffer::anyVBOBound = false;
            }
            
            bool isBound() const {
                return this->bound;
            }
            
            static bool isAnyVBOBound() {
                return anyVBOBound;
            }
        private:
            GLuint id;
            BufferData data;
            std::list<VertexAttribute> attributes;
            
            bool bound;
            static bool anyVBOBound;
        };
        
        bool VertexBuffer::anyVBOBound = false;
    }
}


#endif /* VERTEXBUFFER_H */

