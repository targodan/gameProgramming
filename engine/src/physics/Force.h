#ifndef FORCE_H
#define FORCE_H

#include <eigen3/Eigen/Eigen>
#include "ObjectProperties.h"

namespace engine {
    namespace physics {
        using namespace Eigen;
        
        class Force {
        protected:
            float secondsSinceStart = 0;
            bool enabled = true;
            
        public:
            Force() {}
            virtual ~Force() {}
            
            virtual VectorXf getForceOnVertices(const ObjectProperties& object) = 0;
            
            virtual void addSecondsSinceStart(float secondsSinceStart) {
                this->secondsSinceStart += secondsSinceStart;
            }
            
            virtual void setSecondsSinceStart(float secondsSinceStart) {
                this->secondsSinceStart = secondsSinceStart;
            }
            
            void enable() {
                this->enabled = true;
            }
            void disable() {
                this->enabled = false;
            }
            
            bool isEnabled() const {
                return this->enabled;
            }
        };
    }
}

#endif /* FORCE_H */

