#ifndef BUFFER_H
#define BUFFER_H

#include "BufferType.h"
#include "Bindable.h"
#include "DataUsagePattern.h"
#include "../WTFException.h"
#include "BufferException.h"
#include "gl/gl_core_3_3.h"

#include <easylogging++.h>

namespace engine {
    namespace renderer {
        using namespace gl;
        
        /*
         * Buffer is a simple wrapper for VBOs and EBOs. It does NOT hold the data, 
         * but rather handles it -> it sets up the space on the 
         * graphics card, saves how the graphics card should interpret the data,
         * can load the data to it and finally release the space again.
         * 
         * A buffer-object behaves similar to C-pointers aka it doesn't delete 
         * the data/buffer it's pointing to. You have to manually call releaseBuffer().
         * Copies etc. will all point to the same buffer on the graphics card.
         */
        class Buffer : public Bindable {
        public:
            Buffer(DataUsagePattern usage) 
                : bufferIsValid(false), bufferWasAllocated(false), bound(false), size(0), usage(usage) {
                this->generateBuffer();
            }
            Buffer(const Buffer& orig) = delete;
            Buffer(Buffer&& orig) 
                : id(std::move(orig.id)), bufferIsValid(std::move(orig.bufferIsValid)),
                  bufferWasAllocated(std::move(orig.bufferWasAllocated)), 
                  bound(std::move(orig.bound)), size(std::move(orig.size)),
                  usage(std::move(orig.usage)) {}
            
            Buffer& operator=(Buffer&& right){
                this->id = std::move(right.id);
                this->bufferWasAllocated = std::move(right.bufferWasAllocated);
                this->bound = std::move(right.bound);
                
                return *this;
            }
            
            virtual ~Buffer() {
                this->releaseBuffer();
            }
            
            virtual const BufferType getType() const = 0;
            
            void generateBuffer() {
                glGenBuffers(1, &(this->id));
                this->bufferIsValid = true;
            }
            void releaseBuffer() {
                if(this->bufferIsValid) {
                    glDeleteBuffers(1, &(this->id));
                    this->bufferIsValid = false;
                }
            }
            
            void allocateAndLoadData(const void* data, size_t size) {
#ifdef DEBUG
                if(!this->bufferIsValid) {
                    throw BufferException("The buffer was released.");
                }
                
                if(!this->bound) {
                    // throw BufferException("Could not buffer data: Buffer not bound");
                    LOG(WARNING) << "Trying to load data on a not-bound buffer.";
                }
#endif /*DEBUG*/
                
                glBufferData(this->getType(), size, data, this->usage);
                this->size = size;
                this->bufferWasAllocated = true;
            }
            
            void loadData(const void* data, size_t size, size_t targetBufferOffset = 0) {
#ifdef DEBUG
                if(!this->bufferIsValid) {
                    throw BufferException("The buffer was released.");
                }
                
                if(!this->bound) {
                    // throw BufferException("Could not buffer data: Buffer not bound");
                    LOG(WARNING) << "Trying to load data on a not-bound buffer.";
                }
                
                if(!this->bufferWasAllocated) {
                    throw BufferException("Buffer was not allocated. Allocate first.");
                }
                
                if(size + targetBufferOffset > this->size) {
                    throw BufferException("Trying to load %zu elements with offset %zu into a buffer of size %zu. "
                            "This would overflow the buffer.", size, targetBufferOffset, this->size);
                }
#endif /*DEBUG*/
                
                // This is faster than glBufferData as glBufferData may reallocate new
                // memory on the graphics card.
                // NOTE: glBufferSubData may only ever write less than or equal amounts
                //       of data as the previous glBufferData call.
                glBufferSubData(this->getType(), targetBufferOffset, size, data);
            }
            
            virtual void bind() override {
                if(this->bound) {
                    return;
                }
                
                Bindable::bind(this->getType(), this->id);
                this->bound = true;
            }
            virtual void unbind() override {
                if(!this->bound) {
                    return;
                }
                
                Bindable::unbind(this->getType());
                this->bound = false;
            }
            
            GLuint getID() const {
                return this->id;
            }
            virtual bool isBound() const override {
                return this->bound;
            }
        protected:
            GLuint id;
            
            bool bufferIsValid;
            bool bufferWasAllocated; 
            
            bool bound;
            size_t size;
            DataUsagePattern usage;
        };
    }
}

#endif /* BUFFER_H */

