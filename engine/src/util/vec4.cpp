#include "vec3.h"
#include "vec4.h"

#include "../MathException.h"

#include <easylogging++.h>

namespace engine {
    namespace util {
        vec4::operator vec3() const {
            if(abs(this->w - 1) > 1e-4) {
                LOG(WARNING) << "Trying to convert a vec4 into a vec3 but the 4th vector component is not 1."
                            << "There propably was an invalid transformation! " << *this;
            }
            return vec3(this->x, this->y, this->z);
        }
    }
}