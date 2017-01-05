#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include "VertexAttribute.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "Bindable.h"
#include "BufferException.h"
#include "gl/gl_core_3_3.h"
#include <memory>
#include "../util/vector.h"

namespace engine {
    namespace renderer {
        using util::vector;
        
        class VertexArray : public Bindable {
        public:
            VertexArray() 
                : bound(false) {
                this->generateVertexArray();
            }
            
            VertexArray(vector<std::unique_ptr<VertexBuffer>>&& vbos) 
                : vbos(std::move(vbos)), bound(false) {
                this->generateVertexArray();
            }
            
            VertexArray(vector<std::unique_ptr<VertexBuffer>>&& vbos, std::unique_ptr<ElementBuffer>&& ebo) 
                : vbos(std::move(vbos)), ebo(std::move(ebo)), bound(false) {
                this->generateVertexArray();
            }
                
            VertexArray(const VertexArray& orig) 
                : Bindable(orig), vbos(), id(orig.id), bound(orig.bound) {
                // Note: the copy points to the exact same vao on the graphics card
                // TODO: Is this a good idea? What happens if both the original and the
                //       copy get deconstructed?
                for(auto& vbo : orig.vbos) {
                    this->vbos.push_back(std::make_unique<VertexBuffer>(*vbo));
                }
                
                this->ebo = orig.ebo == nullptr ? nullptr : std::make_unique<ElementBuffer>(*(orig.ebo));
            }
                
            VertexArray(VertexArray&& orig) 
                : Bindable(std::move(orig)), vbos(std::move(orig.vbos)), ebo(std::move(ebo)), id(0), bound(false) {
                // Note: I don't know if this is necessary. When this function returns,
                //       will orig be deleted? If so, its deletion would release its
                //       buffer. Therefore, a new buffer has to be generated.
                // Answer: No it is not necessary, yes orig will be deleted or even be
                //         optimized out completely. Because it is a rvalue, orig will
                //         go out of scope when this method does.
                // TODO: Is generating the array still necessary? Shouldn't this point
                //       to the same array as orig did?
                // Answer: If orig is deleted, its destructor releases the buffer;
                //       Hence, new buffer needed. 
                
                this->generateVertexArray();
            }

            VertexArray& operator=(const VertexArray& right) {
                this->vbos.clear();
                for(auto& vbo : right.vbos) {
                    this->vbos.push_back(std::make_unique<VertexBuffer>(*vbo));
                }
                
                this->id = right.id;
                this->bound = right.bound;
                
                return *this;
            }
            VertexArray& operator=(VertexArray&& right) {
                this->vbos = std::move(right.vbos);
                this->bound = false;
                
                // Note: Releasing is not necessary, orig will be deleted or even be
                //       optimized out completely. Because it is a rvalue, orig will
                //       go out of scope when this method does.
                // TODO: Is generating the array still necessary? Shouldn't this point
                //       to the same array as orig did?
                // Answer: If orig is deleted, its destructor releases the buffer;
                //       Hence, new buffer needed. 
                
                // right.releaseVertexArray();
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
            
            void attachVBO(std::unique_ptr<VertexBuffer>&& vbo) {
                // Is an rvalue so that the user of this function is forced to
                // std::move the unique_ptr in here. Anything will either not work
                // or is unpredictable behaviour.
                vbos.push_back(std::move(vbo));
            }
            
            virtual void bind() override {
                if(this->bound) {
                    return;
                } else if(VertexArray::anyVAOBound) {
                    // TODO: Log warning; maybe bindBuffer to 0?
                    // No error - rebind buffer to this vao
                    VertexArray::anyVAOBound = false; 
                }

                // glBindVertexArray(this->id);
                Bindable::bindVertexArray(this->id);
                VertexArray::anyVAOBound = true;
            }
            virtual void unbind() override {
                if(!this->bound) {
                    return;
                }

                // glBindVertexArray(0);
                Bindable::unbindVertexArray();
                VertexArray::anyVAOBound = false;
            }
            
            virtual bool isBound() const override {
                return this->bound;
            }
            static bool isAnyVAOBound() {
                return anyVAOBound;
            }
            
            void drawArrays() const {
#ifdef DEBUG
                if(!this->bound) {
                    throw BufferException("Could not draw anything. No buffer bound.");
                }
#endif
                glDrawArrays(GL_TRIANGLES, 0, vbos[0]->numberOfElements());
            }
            
            void drawElements() const {
#ifdef DEBUG
                if(!this->bound) {
                    throw BufferException("Could not draw anything. No buffer bound.");
                }
#endif                
                glDrawElements(GL_TRIANGLES, ebo->numberOfElements(), DataType::UINT, (const void*) 0);
            }
        private:          
            void enableAttribute(GLuint index) {
                glEnableVertexAttribArray(index);
            }
            void disableAttribute(GLuint index) {
                glDisableVertexAttribArray(index);
            }
            
            void setAttributePointers(const vector<VertexAttribute>& attributes) {
                for(auto attribute : attributes) {
                    setAttributePointer(attribute);
                    enableAttribute(attribute.index);
                }
            }
            void setAttributePointer(const VertexAttribute& attribute) {
                glVertexAttribPointer(attribute.index, attribute.size, attribute.type, 
                    attribute.normalized, attribute.stride, attribute.offset);
            }
            
            vector<std::unique_ptr<VertexBuffer>> vbos; // For now, only functionality to draw ONE VertexBuffer will be implemented
            std::unique_ptr<ElementBuffer> ebo;
            
            GLuint id;
            
            bool bound;
            static bool anyVAOBound;
        };
    }
}

#endif /* VERTEXARRAY_H */

