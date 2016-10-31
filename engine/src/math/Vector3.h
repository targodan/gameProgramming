#ifndef VECTOR3_H
#define VECTOR3_H

#include "Vector.h"

namespace engine {
    namespace math {
        class Vector3 : public Vector<3> {
        public:
            Vector3();
            Vector3(float x, float y, float z);
            Vector3(const Vector<3>& orig);
            Vector3(const Vector3& orig);
            
            float getX() const;
            float getY() const;
            float getZ() const;
            
            Vector3& setX(float v);
            Vector3& setY(float v);
            Vector3& setZ(float v);
            
            Vector3 cross(const Vector3& v) const;
            
            Vector3& operator=(const Vector<3>& v);
            
            friend Vector3 operator%(const Vector3& v1, const Vector3& v2) {
                return v1.cross(v2);
            }
        };
    }
}

#endif /* VECTOR3_H */

