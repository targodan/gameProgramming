#ifndef VEC3_H
#define VEC3_H

#include <glm/glm.hpp>
#include <google/protobuf/message.h>

#include "pb/vec.pb.h"

namespace engine {
    namespace util {
        class vec2 : public glm::vec2 {
        public:
            vec2() : glm::vec2() {}
            
            vec2& operator=(const vec2& right) {
                glm::vec2::operator=(right);
                return *this;
            }
            
            vec2& operator=(const glm::vec2& right) {
                glm::vec2::operator=(right);
                return *this;
            }
            
            inline void toProtobufMessage(pb::vec& msg) const {
                msg.set_x(this->x);
                msg.set_y(this->y);
            }
            inline void fromProtobufMessage(pb::vec& msg) {
                this->x = msg.x();
                this->y = msg.y();
            }
        };
    }
}

#endif /* VEC3_H */

