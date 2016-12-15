#ifndef VERTEX_H
#define VERTEX_H

#include "util/vec2.h"
#include "util/vec3.h"
#include "gl/gl_core_3_3.h"

namespace engine {
    namespace renderer {
        using namespace util; 
        
        struct Vertex {
            vec3 position;
            vec3 normal;
            vec2 textureCoord;
            
            static const unsigned int posLoc = 0;
            static const unsigned int normLoc = 1;
            static const unsigned int texLoc = 2;
        };
    }
}

#endif /* VERTEX_H */

