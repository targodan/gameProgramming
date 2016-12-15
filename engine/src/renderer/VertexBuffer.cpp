#include "VertexBuffer.h"
#include "../WTFException.h"

namespace engine {
    namespace renderer {
        bool VertexBuffer::bound = false;
        
        VertexBuffer::VertexBuffer() : id(0) {
            glGenBuffers(1, &(this->id));
        }

        VertexBuffer::~VertexBuffer() {
            glDeleteBuffers(1, &(this->id));
        }
        
        const GLuint* VertexBuffer::getIDPtr() const {
            return &(this->id);
        }
        
        void VertexBuffer::bind() const {
            if(VertexBuffer::bound) {
                throw WTFException("Could not bind VBO. Some VBO is already bound.");
            }
            
            glBindBuffer(GL_ARRAY_BUFFER, this->id);
            VertexBuffer::bound = true;
        }
        
        void VertexBuffer::unbind() const {
            if(!VertexBuffer::bound) {
                throw WTFException("Could not unbind VBO. No VBO bound.");
            }
            
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            VertexBuffer::bound = false;
        }
        
        void VertexBuffer::setAttributes(const std::list<VertexAttribute>& attribs) {
            this->attributes = attribs;
        }
        
        const std::list<VertexAttribute>& VertexBuffer::getAttributes() const {
            return this->attributes;
        }
    }
}
