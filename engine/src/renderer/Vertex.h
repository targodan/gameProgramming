#ifndef VERTEX_H
#define VERTEX_H

#include "util/vec2.h"
#include "util/vec3.h"

namespace engine {
    namespace ogl {
        using namespace util; 
        
        struct Vertex {
            vec3 position;
            vec3 normal;
            vec2 textureCoord;
        };
    }
}

#endif /* VERTEX_H */

