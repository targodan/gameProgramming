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
        
        /*
         * VertexArray wraps the usage of VAOs which define how a set VBOs needs
         * to be interpreted to correctly draw the vertices stored in them on the
         * screen.
         * 
         * - The corresponding VAO on the graphics card is generated on creation
         *   of an VertexArray-object, and released on its deletion
         * - Copies will be associated with a NEW VAO on the graphics card 
         */
        class VertexArray : public Bindable {
        public:
            VertexArray() 
                : vbos(), ebo(), bound(false) {
                this->generateVertexArray();
            }
            VertexArray(vector<std::unique_ptr<VertexBuffer>>&& vbos) 
                : vbos(std::move(vbos)), ebo(), bound(false) {
                this->generateVertexArray();
            }
            VertexArray(vector<std::unique_ptr<VertexBuffer>>&& vbos, std::unique_ptr<ElementBuffer>&& ebo) 
                : vbos(std::move(vbos)), ebo(std::move(ebo)), bound(false) {
                this->generateVertexArray();
            }
                
            VertexArray(const VertexArray& orig) 
                : Bindable(orig), vbos(), bound(false) {
                this->generateVertexArray();
                
                for(auto& vbo : orig.vbos) {
                    this->vbos.push_back(std::make_unique<VertexBuffer>(*vbo));
                }
                
                this->ebo = orig.ebo == nullptr ? nullptr : std::make_unique<ElementBuffer>(*(orig.ebo));
            }
            VertexArray(VertexArray&& orig) 
                : Bindable(std::move(orig)), vbos(std::move(orig.vbos)), ebo(std::move(ebo)), bound(false) {
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
                this->ebo = right.ebo == nullptr ? nullptr : std::make_unique<ElementBuffer>(*(right.ebo));
                
                this->bound = false;
                this->generateVertexArray();
                
                return *this;
            }
            VertexArray& operator=(VertexArray&& right) {
                this->vbos = std::move(right.vbos);
                this->ebo = std::move(right.ebo);
                
                this->bound = false;
                this->generateVertexArray();
                
                return *this;
            }
            
            ~VertexArray() {
                if(this->generated) {
                    this->releaseVertexArray();
                }
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
                vbos.push_back(std::move(vbo));
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
                
                this->bound = true;
                VertexArray::anyVAOBound = true;
            }
            virtual void unbind() override {
                if(!this->bound) {
                    return;
                }

                // glBindVertexArray(0);
                Bindable::unbindVertexArray();
                
                this->bound = false;
                VertexArray::anyVAOBound = false;
            }
            
            virtual bool isBound() const override {
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

