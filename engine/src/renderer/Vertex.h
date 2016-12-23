#ifndef VERTEX_H
#define VERTEX_H

#include "../util/vec2.h"
#include "../util/vec3.h"
#include "gl/gl_core_3_3.h"

namespace engine {
    namespace renderer {
        using namespace util; 
        
        struct Vertex {
            vec3 position;
            vec3 normal;
            vec2 textureCoord;
            
            Vertex() {}
            Vertex(vec3 position, vec3 normal = vec3(), vec2 textureCoord = vec2())
                : position(position), normal(normal), textureCoord(textureCoord) {}
            
            Vertex(const Vertex& orig) : position(orig.position), normal(orig.normal), textureCoord(orig.textureCoord) {}
            Vertex(Vertex&& orig) : position(std::move(orig.position)), normal(std::move(orig.normal)), textureCoord(std::move(orig.textureCoord)) {}

            Vertex& operator=(const Vertex& right) {
                this->position = right.position;
                this->normal = right.normal;
                this->textureCoord = right.textureCoord;
                return *this;
            }
            Vertex& operator=(Vertex&& right) {
                std::swap(this->position, right.position);
                std::swap(this->normal, right.normal);
                std::swap(this->textureCoord, right.textureCoord);
                return *this;
            }
            
            static const unsigned int posLoc = 0;
            static const unsigned int normLoc = 1;
            static const unsigned int texLoc = 2;
        };
    }
}

#endif /* VERTEX_H */

