#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include "VertexAttribute.h"
#include "VertexBuffer.h"
#include "gl/gl_core_3_3.h"
#include <memory>
#include "util/vector.h"
#include <list>

namespace engine {
    namespace renderer {
        using util::vector;
        
        class VertexArray {
        public:
            VertexArray() 
                : bound(false) {
                this->generateVertexArray();
            }
            
            VertexArray(const std::list<std::unique_ptr<VertexBuffer>> vbos) 
                : vbos(vbos), bound(false) {
                    this->generateVertexArray();
            }
                
            VertexArray(const VertexArray& orig) 
                : vbos(orig.vbos), id(orig.id), bound(orig.bound) {
                // Note: the copy points to the exact same vao on the graphics card
            }
                
            VertexArray(VertexArray&& orig) 
                : vbos(std::move(orig.vbos)), id(std::move(orig.id)), bound(std::move(orig.bound)) {
                // Note: I don't know if this is necessary. When this function returns,
                //       will orig be deleted? If so, its deletion would release its
                //       buffer. Therefore, a new buffer has to be generated.
                
                orig.releaseVertexArray();
                this->generateVertexArray();
            }

            VertexArray& operator=(const VertexArray& right) {
                this->vbos = right.vbos;
                this->id = right.id;
                this->bound = right.bound;
                
                return *this;
            }
            VertexArray& operator=(VertexArray&& right) {
                this->vbos = std::move(right.vbos);
                this->bound = std::move(right.bound);
                
                right.releaseVertexArray();
                this->generateVertexArray();
                
                return *this;
            }
            
            ~VertexArray() {
                this->releaseVertexArray();
            }
            
            void generateVertexArray() {
                glGenVertexArrays(1, &(this->id));
            }
            void releaseVertexArray() {
                glDeleteVertexArrays(1, &(this->id));
            }
            
            void loadData() {
                for(auto& vbo : this->vbos) {
                    vbo->bind();
                    vbo->loadData();
                    vbo->bind();
                }
            }
            
            void setAttributePointers() {
                for(auto& vbo : this->vbos) {
                    vbo->bind();
                    setAttributePointers(vbo->getAttributes());
                    vbo->unbind();
                }
            }
            
            void attachVBO(std::unique_ptr<VertexBuffer> vbo) {
                vbos.push_back(std::move(vbo));
            }
            
            void bind() const {
                if(this->bound) {
                    return;
                } else if(VertexArray::anyVAOBound) {
                    // TODO: Log warning; maybe bindBuffer to 0?
                    // No error - rebind buffer to this vao
                    VertexArray::anyVAOBound = false; 
                }

                glBindVertexArray(this->id);
                VertexArray::anyVAOBound = true;
            }
            void unbind() const {
                if(!this->bound) {
                    return;
                }

                glBindVertexArray(0);
                VertexArray::anyVAOBound = false;
            }
            
            bool isBound() const {
                return this->bound;
            }
            static bool isAnyVAOBound() {
                return anyVAOBound;
            }
        private:
            void enableAttribute(GLuint index) {
                glEnableVertexAttribArray(index);
            }
            void disableAttribute(GLuint index) {
                glDisableVertexAttribArray(index);
            }
            
            void setAttributePointers(const std::list<VertexAttribute>& attributes) {
                for(auto attribute : attributes) {
                    setAttributePointer(attribute);
                    enableAttribute(attribute.index);
                }
            }
            void setAttributePointer(const VertexAttribute& attribute) {
                glVertexAttribPointer(attribute.index, attribute.size, attribute.type, 
                    attribute.normalized, attribute.stride, attribute.offset);
            }
            
            std::list<std::unique_ptr<VertexBuffer>> vbos;
            GLuint id;
            
            bool bound;
            static bool anyVAOBound;
        };
        
        bool VertexArray::anyVAOBound = false;
    }
}

#endif /* VERTEXARRAY_H */

