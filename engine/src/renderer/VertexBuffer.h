#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include "VertexAttribute.h"
#include "gl/gl_core_3_3.h"
#include <list>

namespace engine {
    namespace renderer {
        using namespace gl;
        
        class VertexBuffer {
        public:
            VertexBuffer();
            VertexBuffer(const VertexBuffer& orig) = delete;
            virtual ~VertexBuffer();
            
            const GLuint* getIDPtr() const;
            void bind() const;
            void unbind() const;  // TODO: make static?
            void setAttributes(const std::list<VertexAttribute>& attribs);
            const std::list<VertexAttribute>& getAttributes() const;
            
            static const bool isBound() {
                return bound;
            }
        private:
            GLuint id;
            
            std::list<VertexAttribute> attributes; 
            
            static bool bound;
        };        
    }
}


#endif /* VERTEXBUFFER_H */

