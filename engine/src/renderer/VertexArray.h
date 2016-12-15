#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include "VertexAttribute.h"
#include "VertexBuffer.h"
#include "gl/gl_core_3_3.h"
#include <memory>

namespace engine {
    namespace renderer {
        class VertexArray {
        public:
            VertexArray();
            VertexArray(const VertexArray& orig) = delete;
            virtual ~VertexArray();
            
            void setAttributePointers();
            
            void bind() const;
            void unbind() const; // TODO: make static
            
            void enableAttribute(GLuint index);
            void disableAttribute(GLuint index);
            
            static const bool isBound() {
                return bound;
            }
        private:
            void setAttributePointers(const std::list<VertexAttribute>& attributes);
            void setAttributePointer(const VertexAttribute& attribute);
            
            std::list<std::unique_ptr<VertexBuffer>> vbos;
            GLuint id;
            
            static bool bound;
        };
    }
}

#endif /* VERTEXARRAY_H */

