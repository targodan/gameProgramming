#ifndef BUFFER_H
#define BUFFER_H

#include "BufferType.h"
#include "Bindable.h"
#include "DataUsagePattern.h"
#include "../WTFException.h"
#include "gl/gl_core_3_3.h"

namespace engine {
    namespace renderer {
        using namespace gl;
        
        struct BufferData {
            const void* dataPtr;
            size_t size; // size in bytes
            size_t nElements; // no of elements (cannot be calculated here because data type is not known)
            DataUsagePattern usage;
        };
        
        
        /*
         * Buffer is a simple wrapper for VBOs and EBOs. It does NOT hold the data, 
         * but rather handles it -> it sets up the space on the 
         * graphics card, saves how the graphics card should interpret the data,
         * can load the data to it and finally release the space again.
         * 
         * If a Buffer-Object is copied or moved, ONLY the data-struct will be
         * copied/moved (its pointer to the actual data therefore points to the
         * same address as before). A new buffer will be generated on the graphics
         * card for the copy/moved instance; if necessary, the data will be loaded
         * into this new buffer.
         */
        class Buffer : public Bindable {
        public:
            Buffer() 
                : loadedToGraphicsCard(false), bound(false), data {nullptr, 0, 0, DataUsagePattern::STATIC_DRAW} {
                this->generateBuffer();
            }
            Buffer(const void* dataPtr, size_t size, size_t nElements, DataUsagePattern usage) 
                : loadedToGraphicsCard(false), bound(false), data {dataPtr, size, nElements, usage} {
                this->generateBuffer();
            }
            Buffer(const Buffer& orig) : loadedToGraphicsCard(false), bound(false), data(orig.data) {
                this->generateBuffer();
                
                if(orig.loadedToGraphicsCard) {
                    this->bind();
                    this->loadData();
                    this->unbind();
                }
            }
            Buffer(Buffer&& orig) : id(0), loadedToGraphicsCard(false), bound(false), data(std::move(orig.data)) {
                // Note: I don't know if this is necessary. When this function returns,
                //       will orig be deleted? If so, its deletion would release its
                //       buffer. Therefore, a new buffer has to be generated.
                // Answer: No it is not necessary, yes orig will be deleted or even be
                //         optimized out completely. Because it is a rvalue, orig will
                //         go out of scope when this method does.
                // TODO: Is generating the buffer still necessary? Shouldn't this point
                //       to the same buffer as orig did?
                // Answer: If orig is deleted, its destructor releases the buffer (see below);
                //       Hence, new buffer needed. 
                this->generateBuffer();
                
                if(orig.loadedToGraphicsCard) {
                    this->bind();
                    this->loadData();
                    this->unbind();
                }
            }
            virtual ~Buffer() {
                this->releaseBuffer();
            }
            
            virtual const BufferType getType() const = 0;
            
            void generateBuffer() {
                glGenBuffers(1, &(this->id));
            }
            void releaseBuffer() {
                glDeleteBuffers(1, &(this->id));
            }
            
            void loadData() {
#ifdef DEBUG
                if(!this->data.dataPtr) {
                    throw BufferException("Could not buffer data: No data specified");
                } else if(!this->bound) {
                    throw BufferException("Could not buffer data: Buffer not bound");
                }

                if(this->loadedToGraphicsCard) {
                    // TODO: Log warning: currently loaded data will be overwritten
                }
#endif /*DEBUG*/
                
                glBufferData(this->getType(), data.size, data.dataPtr, data.usage);
                this->loadedToGraphicsCard = true;
            }
            void loadData(const void* data, size_t size, size_t nElements, DataUsagePattern usage) {
#ifdef DEBUG
                if(data == nullptr) {
                    throw BufferException("Could not buffer data: No data specified");
                } else if(!this->bound) {
                    throw BufferException("Could not buffer data: Buffer not bound");
                }

                if(this->loadedToGraphicsCard) {
                    // TODO: Log warning: currently loaded data will be overwritten
                }
#endif /*DEBUG*/
                
                glBufferData(this->getType(), size, data, usage);
                this->loadedToGraphicsCard = true;
                
                this->data = {data, size, nElements, usage};
            }
            
            virtual void bind() override {
                Bindable::bind(this->getType(), this->id);
            }
            virtual void unbind() override {
                Bindable::unbind(this->getType());
            }
            
            size_t numberOfElements() const {
                return this->data.nElements;
            }
            GLuint getID() const {
                return this->id;
            }
            virtual bool isBound() const override {
                return this->bound;
            }
        protected:
            GLuint id;
            
            bool loadedToGraphicsCard; 
            bool bound;
            BufferData data;
        };
    }
}

#endif /* BUFFER_H */

