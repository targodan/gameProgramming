#include "Vector3.h"

namespace engine {
    namespace math {
        Vector3::Vector3() : Vector<3>() {}
        
        Vector3::Vector3(float x, float y, float z)
                : Vector<3>({x, y, z}) {}

        Vector3::Vector3(const Vector<3>& orig) {
            std::memcpy(this->elements, orig.elements, 3 * sizeof(float));
        }

        Vector3::Vector3(const Vector3& orig) {
            std::memcpy(this->elements, orig.elements, 3 * sizeof(float));
        }

        Vector3 Vector3::cross(const Vector3& v2) const {
            Vector3 ret;
            const Vector3& v1 = *this;
            ret.elements[0] = v1.elements[1] * v2.elements[2] - v1.elements[2] * v2.elements[1];
            ret.elements[1] = v1.elements[2] * v2.elements[0] - v1.elements[0] * v2.elements[2];
            ret.elements[2] = v1.elements[0] * v2.elements[1] - v1.elements[1] * v2.elements[0];
            return ret;
        }
        
        float Vector3::getX() const {
            return this->elements[0];
        }

        float Vector3::getY() const {
            return this->elements[1];
        }

        float Vector3::getZ() const {
            return this->elements[2];
        }

        Vector3& Vector3::setX(float v) {
            this->elements[0] = v;
            return *this;
        }

        Vector3& Vector3::setY(float v) {
            this->elements[1] = v;
            return *this;
        }

        Vector3& Vector3::setZ(float v) {
            this->elements[2] = v;
            return *this;
        }
        
        Vector3& Vector3::operator=(const Vector<3>& v) {
            std::memcpy(this->elements, v.elements, 3 * sizeof(float));
            return *this;
        }
        
        Vector3& Vector3::operator=(const Vector3& v) {
            std::memcpy(this->elements, v.elements, 3 * sizeof(float));
            return *this;
        }
    }
}