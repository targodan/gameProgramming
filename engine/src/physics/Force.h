#ifndef FORCE_H
#define FORCE_H

#include <eigen3/Eigen/Eigen>

namespace engine {
    namespace physics {
        using namespace Eigen;
        
        class Force {
        protected:
            float secondsSinceStart = 0;
            
        public:
            Force() {}
            virtual ~Force() {}
            
            virtual Matrix<float, Dynamic, 1> getForceOnVertices(const Matrix<float, Dynamic, 1>& verticies) const = 0;
            
            virtual void addSecondsSinceStart(float secondsSinceStart) {
                this->secondsSinceStart += secondsSinceStart;
            }
            
            virtual void setSecondsSinceStart(float secondsSinceStart) {
                this->secondsSinceStart = secondsSinceStart;
            }
        };
    }
}

#endif /* FORCE_H */

