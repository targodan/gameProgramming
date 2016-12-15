#include "VertexArray.h"
#include "../WTFException.h"

namespace engine {
    namespace renderer {
        bool VertexArray::bound = false;
        
        VertexArray::VertexArray() : id(0) {
            glGenVertexArrays(1, &(this->id));
        }


        VertexArray::~VertexArray() {

        }
        
        void VertexArray::setAttributePointers() {
            this->bind();
            for(auto& vbo : this->vbos) {
                vbo->bind();
                setAttributePointers(vbo->getAttributes());
                vbo->unbind();
            }
            this->unbind();
        }
        
        void VertexArray::setAttributePointers(const std::list<VertexAttribute>& attributes) {
            for(auto attribute : attributes) {
                setAttributePointer(attribute);
                enableAttribute(attribute.index);
            }
        }
        
        void VertexArray::setAttributePointer(const VertexAttribute& attribute) {
            glVertexAttribPointer(attribute.index, attribute.size, attribute.type, 
                    attribute.normalized, attribute.stride, attribute.offset);
        }
        
        void VertexArray::bind() const {
            if(VertexArray::bound) {
                throw WTFException("Could not bind VAO. Some VAO is already bound.");
            }
            
            glBindVertexArray(this->id);
            VertexArray::bound = true;
        }
        
        void VertexArray::unbind() const {
            if(!VertexArray::bound) {
                throw WTFException("Could not unbind VAO. No VAO bound.");
            }
            
            glBindVertexArray(0);
            VertexArray::bound = false;
        }
        
        void VertexArray::disableAttribute(GLuint index) {
            glDisableVertexAttribArray(index);
        }

        void VertexArray::enableAttribute(GLuint index) {
            glEnableVertexAttribArray(index);
        }

    }
}
