#include "ostream_helper.h"

std::ostream& operator<<(std::ostream& os, const glm::vec2& vec) {
    os << "(";
    for(size_t i = 0; i < vec.length(); ++i) {
        os << vec[i];
        if(i+1 < vec.length()) {
            os << ", ";
        }
    }
    os << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const glm::vec3& vec) {
    os << "(";
    for(size_t i = 0; i < vec.length(); ++i) {
        os << vec[i];
        if(i+1 < vec.length()) {
            os << ", ";
        }
    }
    os << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const glm::vec4& vec) {
    os << "(";
    for(size_t i = 0; i < vec.length(); ++i) {
        os << vec[i];
        if(i+1 < vec.length()) {
            os << ", ";
        }
    }
    os << ")";
    return os;
}
