#ifndef TETRAHEDRONIZER_H
#define TETRAHEDRONIZER_H

#include "TetrahedronizedObject.h"

namespace engine {
    namespace physics {
        using Eigen::VectorXf;
        using Eigen::Vector3f;
        using Eigen::MatrixXf;
        using Eigen::Hyperplane;
        
        class Tetrahedronizer {
        public:
            
            static TetrahedronizedObject tetrahedronizeCuboid(
                    const Vector3f& corner,
                    const Vector3f& spanVectorOne, const Vector3f& spanVectorTwo, const Vector3f& spanVectorThree,
                    size_t numTetrahedronAlongSpanOne, size_t numTetrahedronAlongSpanTwo, size_t numTetrahedronAlongSpanThree 
            );
        };
    }
}

#endif /* TETRAHEDRONIZER_H */

