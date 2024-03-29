#ifndef VEC2_H
#define VEC2_H

#include <glm/glm.hpp>
#include <google/protobuf/message.h>

#include "pb/vec.pb.h"

namespace engine {
    namespace util {
        class vec2 : public glm::vec2 {
        public:
            vec2() : glm::vec2() {}
            vec2(float x, float y) : glm::vec2(x, y) {}
            
            vec2(const glm::vec2& orig) : glm::vec2(orig) {}
            vec2(const vec2& orig) : glm::vec2(orig) {}
            vec2(vec2&& orig) : glm::vec2(std::move(orig)) {}
            
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
            
            friend std::ostream& operator<<(std::ostream& os, const vec2& vec) {
                os << "(" << vec.x << ", " << vec.y << ")";
                return os;
            }
        };
    }
}

#endif /* VEC2_H */

