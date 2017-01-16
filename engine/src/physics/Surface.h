#ifndef SURFACE_H
#define SURFACE_H

#include <eigen3/Eigen/Eigen>

namespace engine {
    namespace physics {
        using namespace Eigen;
        
        struct Surface {
            Surface(const Matrix<float, Dynamic, 1>& vertices,
                    const Matrix<float, Dynamic, 1>& surfaceAreaPerVertex)
                    : vertices(vertices), surfaceAreaPerVertex(surfaceAreaPerVertex) {}
            
            Matrix<float, Dynamic, 1> vertices;
            Matrix<float, Dynamic, 1> surfaceAreaPerVertex;
        };
    }
}

#endif /* SURFACE_H */

