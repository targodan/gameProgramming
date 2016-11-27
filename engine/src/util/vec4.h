#ifndef VEC4_H
#define VEC4_H

#include <glm/glm.hpp>
#include <google/protobuf/message.h>

#include "pb/vec.pb.h"

namespace engine {
    namespace util {
        class vec4 : public glm::vec4 {
        public:
            vec4() : glm::vec4() {}
            
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
        };
    }
}

#endif /* VEC3_H */

