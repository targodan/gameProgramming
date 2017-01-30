#ifndef VEC4_H
#define VEC4_H

#include <glm/glm.hpp>
#include <eigen3/Eigen/Eigen>
#include <google/protobuf/message.h>
#include <iostream>

#include "pb/vec.pb.h"

namespace engine {
    namespace util {
        class vec3;
        
        class vec4 : public glm::vec4 {
        public:
            vec4() : glm::vec4() {}
            vec4(const Eigen::Vector4f& v) : vec4(v[0], v[1], v[2], v[3]) {}
            vec4(float x, float y, float z, float w) : glm::vec4(x, y, z, w) {}
            
            vec4(const glm::vec4& orig) : glm::vec4(orig) {}
            vec4(const vec4& orig) : glm::vec4(orig) {}
            vec4(vec4&& orig) : glm::vec4(std::move(orig)) {}
            
            vec4& operator=(const vec4& right) {
                glm::vec4::operator=(right);
                return *this;
            }
            
            vec4& operator=(const glm::vec4& right) {
                glm::vec4::operator=(right);
                return *this;
            }
            
            inline void toProtobufMessage(pb::vec& msg) const {
                msg.set_x(this->x);
                msg.set_y(this->y);
                msg.set_z(this->z);
                msg.set_w(this->w);
            }
            inline void fromProtobufMessage(pb::vec& msg) {
                this->x = msg.x();
                this->y = msg.y();
                this->z = msg.z();
                this->w = msg.w();
            }
            
            friend std::ostream& operator<<(std::ostream& os, const vec4& vec) {
                os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
                return os;
            }
            
            operator vec3() const;
        };
    }
}

#endif /* VEC3_H */

