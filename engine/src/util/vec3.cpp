#include "vec3.h"
#include "vec4.h"

namespace engine {
    namespace util {
        vec3::operator vec4() const {
            return vec4(this->x, this->y, this->z, 1);
        }
    }
}