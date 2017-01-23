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
                : Bindable(orig), vbos(), id(orig.id), bound(orig.bound) {
                for(auto& vbo : orig.vbos) {
                    this->vbos.push_back(std::make_unique<VertexBuffer>(*vbo));
                }
                
                this->ebo = orig.ebo == nullptr ? nullptr : std::make_unique<ElementBuffer>(*(orig.ebo));
            }
            VertexArray(VertexArray&& orig) 
                : Bindable(std::move(orig)), vbos(std::move(orig.vbos)), ebo(std::move(orig.ebo)), id(std::move(orig.id)), bound(std::move(orig.bound)) {
                // this->ebo = orig.ebo == nullptr ? nullptr : std::make_unique<ElementBuffer>(*(orig.ebo));
            }

            VertexArray& operator=(const VertexArray& right) {
                this->vbos.clear();
                for(auto& vbo : right.vbos) {
                    this->vbos.push_back(std::make_unique<VertexBuffer>(*vbo));
                }
                this->ebo = right.ebo == nullptr ? nullptr : std::make_unique<ElementBuffer>(*(right.ebo));
               
                this->id = right.id;
                this->bound = right.bound;
                
                return *this;
            }
            VertexArray& operator=(VertexArray&& right) {
                this->vbos = std::move(right.vbos);
                this->ebo = std::move(right.ebo);
                this->id = std::move(right.id);
                this->bound = std::move(right.bound);
                
                return *this;
            }
            
            ~VertexArray() {
                
            }
            
            void generateVertexArray() {
                glGenVertexArrays(1, &(this->id));
            }
            void releaseVertexArray() {
                for(auto& vbo : this->vbos) {
                    vbo->releaseBuffer();
                }
                
                if(ebo != nullptr) {
                    ebo->releaseBuffer();
                }
                
                glDeleteVertexArrays(1, &(this->id));
            }
            
            void loadData() {
                for(auto& vbo : this->vbos) {
                    vbo->bind();
                    vbo->loadData();
                    vbo->unbind();
                }
            }
            
            void reloadData() {
                for(auto& vbo : this->vbos) {
                    vbo->bind();
                    vbo->reloadData();
                    vbo->unbind();
                }
            }
            void loadIndices() {
                ebo->loadData();
            }
            void reloadIndices() {
                ebo->reloadData();
            }
            
            void setAttributePointers() {
#ifdef DEBUG
                if(!this->bound) {
                    throw WTFException("Cannot set vertex attribute pointers: no VAO bound.");
                }
#endif /*DEBUG*/
                for(auto& vbo : this->vbos) {
                    vbo->bind();
                    vbo->setAttributePointers();
                    vbo->unbind();
                }
            }
            
            void drawArrays() const {
#ifdef DEBUG
                if(!this->bound) {
                    throw WTFException("Could not draw anything. No buffer bound.");
                }
#endif
                glDrawArrays(GL_TRIANGLES, 0, vbos[0]->numberOfElements());
            }    
            void drawElements() const {
#ifdef DEBUG
                if(!this->bound) {
                    throw WTFException("Could not draw anything. No buffer bound.");
                }
#endif                
                glDrawElements(GL_TRIANGLES, ebo->numberOfElements(), DataType::UINT, (const void*) 0);
            }
            
            virtual void bind() override {
                if(this->bound) {
                    return;
                } else if(VertexArray::anyVAOBound) {
                    LOG(WARNING) << "Trying to bind a VAO, but there is one bound already.";
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
            
            void attachVBO(std::unique_ptr<VertexBuffer>&& vbo) {
                vbos.push_back(std::move(vbo));
            }
            const vector<std::unique_ptr<VertexBuffer>>& getVBOs() const {
                return this->vbos;
            }
            
            void setEBO(std::unique_ptr<ElementBuffer> ebo) {
                this->ebo = std::move(ebo);
                
                // This neat little trick seems to store the ebo in the vao,
                // relieving the user of the necessity to ever bind this ebo again 
                this->bind();
                this->ebo->bind();
                this->unbind();
                this->ebo->unbind();
            }
        private:
            vector<std::unique_ptr<VertexBuffer>> vbos; // For now, only functionality to draw ONE VertexBuffer will be implemented
            std::unique_ptr<ElementBuffer> ebo;
            
            GLuint id;
            
            bool bound;
            static bool anyVAOBound;
        };
    }
}

#endif /* VERTEXARRAY_H */

