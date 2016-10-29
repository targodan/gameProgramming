#include "Vector3.h"

namespace engine {
    namespace math {
        Vector3::Vector3() {}
        
        Vector3::Vector3(float x, float y, float z)
                : Vector<3>(x, y, z) {}

        Vector3::Vector3(const Vector3& orig) {
            for(int i = 0; i < 3; ++i) {
                this->elements[i] = orig.elements[i];
            }
        }

        Vector3 Vector3::cross(const Vector3& v2) const {
            Vector3 ret;
            Vector3& v1 = *this;
            ret[0] = v1[1] * v2[2] - v1[2] * v2[1];
            ret[1] = v1[2] * v2[0] - v1[0] * v2[2];
            ret[2] = v1[0] * v2[1] - v1[1] * v2[0];
            return ret;
        }
    }
}