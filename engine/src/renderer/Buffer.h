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
        
        class Buffer : public Bindable {
        public:
            Buffer() 
                : bound(false), data {nullptr, 0, 0, DataUsagePattern::STATIC_DRAW} {
                this->generateBuffer();
            }
            Buffer(const void* dataPtr, size_t size, size_t nElements, DataUsagePattern usage) 
                : bound(false), data {dataPtr, size, nElements, usage} {
                this->generateBuffer();
            }
            Buffer(const Buffer& orig) : id(orig.id), bound(orig.bound), data(orig.data) {
                // Note: the copy points to the exact same buffer on the graphics card
                // TODO: Is this a good idea? What happens if both the original and the
                //       copy get deconstructed?
            }
            Buffer(Buffer&& orig) : id(0), bound(false), data(std::move(orig.data)) {
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
                if(!this->data.dataPtr) {
                    throw WTFException("Could not buffer data: No data specified");
                }
                
                glBufferData(this->getType(), data.size, data.dataPtr, data.usage);
            }
            void loadData(const void* data, size_t size, size_t nElements, DataUsagePattern usage) {
                glBufferData(this->getType(), size, data, usage);
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
            
            bool bound;
            BufferData data;
        };
    }
}

#endif /* BUFFER_H */

