#ifndef VEC3_H
#define VEC3_H

#include <glm/glm.hpp>
#include <eigen3/Eigen/Eigen>
#include <google/protobuf/message.h>

#include "pb/vec.pb.h"

namespace engine {
    namespace util {
        class vec4;
        
        class vec3 : public glm::vec3 {
        public:
            vec3() : glm::vec3() {}
            vec3(const Eigen::Vector3f& v) : vec3(v[0], v[1], v[2]) {}
            vec3(float x, float y, float z) : glm::vec3(x, y, z) {}
            
            vec3(const glm::vec3& orig) : glm::vec3(orig) {}
            vec3(const vec3& orig) : glm::vec3(orig) {}
            vec3(vec3&& orig) : glm::vec3(std::move(orig)) {}
            
            vec3& operator=(const vec3& right) {
                glm::vec3::operator=(right);
                return *this;
            }
            
            vec3& operator=(const glm::vec3& right) {
                glm::vec3::operator=(right);
                return *this;
            }
            
            inline pb::vec* toProtobufMessage() const {
                pb::vec* msg = new pb::vec;
                msg->set_x(this->x);
                msg->set_y(this->y);
                msg->set_z(this->z);
                return msg;
            }
            inline void fromProtobufMessage(const pb::vec& msg) {
                this->x = msg.x();
                this->y = msg.y();
                this->z = msg.z();
            }
            
            friend std::ostream& operator<<(std::ostream& os, const vec3& vec) {
                os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
                return os;
            }
            
            operator vec4() const;
        };
    }
}

#endif /* VEC3_H */

