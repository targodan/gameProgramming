#ifndef MESH_H
#define MESH_H

#include "../util/Array.h"
#include "Vertex.h"
#include "gl/gl_core_3_3.h"

namespace engine {
    namespace renderer {
        using namespace util;
        using namespace gl;
        
        class Mesh {
        public:
            Mesh(Array<Vertex> vertices, Array<GLuint> indices, GLenum usage = GL_STATIC_DRAW);
            
            Mesh(const Mesh& orig);
            Mesh(Mesh&& orig);

            Mesh& operator=(const Mesh& right);
            Mesh& operator=(Mesh&& right);
            
            virtual ~Mesh();
            
            std::string getUsage() const;
            
        private:
            GLenum usage;
            
            GLuint vao, vbo;
            
            Array<Vertex> vertices;
            Array<GLuint> indices;
            
            void initMesh();
            void releaseMesh();
        };
    }
}


#endif /* MESH_H */

