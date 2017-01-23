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
         * A buffer-object behaves similar to C-pointers aka it doesn't delete 
         * the data/buffer it's pointing to. You have to manually call releaseBuffer().
         * Copies etc. will all point to the same buffer on the graphics card.
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
            Buffer(const Buffer& orig) 
                : id(orig.id), loadedToGraphicsCard(orig.loadedToGraphicsCard), bound(orig.bound), data(orig.data) {
                
            }
            Buffer(Buffer&& orig) 
                : id(std::move(orig.id)), loadedToGraphicsCard(std::move(orig.loadedToGraphicsCard)), 
                  bound(std::move(orig.bound)), data(std::move(orig.data)) {
                
            }
            
            Buffer& operator=(const Buffer& right) {
                this->id = right.id;
                this->loadedToGraphicsCard = right.loadedToGraphicsCard;
                this->bound = right.bound;
                this->data = right.data;
                
                return *this;
            }
            Buffer& operator=(Buffer&& right){
                this->id = std::move(right.id);
                this->loadedToGraphicsCard = std::move(right.loadedToGraphicsCard);
                this->bound = std::move(right.bound);
                this->data = std::move(right.data);
                
                return *this;
            }
            
            virtual ~Buffer() {
            
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
                    // throw BufferException("Could not buffer data: Buffer not bound");
                    LOG(WARNING) << "Trying to load data on a not-bound buffer.";
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
                    // throw BufferException("Could not buffer data: Buffer not bound");
                    LOG(WARNING) << "Trying to load data on a not-bound buffer.";
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
            
            size_t numberOfElements() const {
                return this->data.nElements;
            }
            GLuint getID() const {
                return this->id;
            }
            virtual bool isBound() const override {
                return this->bound;
            }
            bool isLoadedToGraphicsCard() const {
                return this->loadedToGraphicsCard;
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

