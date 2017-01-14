#ifndef OSTREAM_HELPER_H
#define OSTREAM_HELPER_H

#include <iostream>
#include <vector>
#include <type_traits>
#include <memory>
#include <glm/glm.hpp>

namespace engine {
    template<typename T>
    struct is_pointer_helper : std::false_type {};
    template<typename T>
    struct is_pointer_helper<T*> : std::true_type {};
    template<typename T>
    struct is_pointer_helper<std::shared_ptr<T>> : std::true_type {};
    template<typename T>
    struct is_pointer_helper<std::unique_ptr<T>> : std::true_type {};
    template<typename T>
    struct is_pointer_helper<std::weak_ptr<T>> : std::true_type {};
    
    template<typename T>
    struct is_pointer : is_pointer_helper<typename std::remove_cv<T>::type> {};
}

std::ostream& operator<<(std::ostream& os, const glm::vec2& vec);
std::ostream& operator<<(std::ostream& os, const glm::vec3& vec);
std::ostream& operator<<(std::ostream& os, const glm::vec4& vec);

template<class T, typename std::enable_if<engine::is_pointer<T>::value, bool>::type = 0>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& list) {
    os << "[";
    for(auto it = list.begin(); it != list.end(); ++it) {
        os << **it;
        if(it < list.end()-1) {
            os << ", ";
        }
    }
    os << "]";
    return os;
}

template<class T, typename std::enable_if<!engine::is_pointer<T>::value, bool>::type = 0>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& list) {
    os << "[";
    for(auto it = list.begin(); it != list.end(); ++it) {
        os << *it;
        if(it < list.end()-1) {
            os << ", ";
        }
    }
    os << "]";
    return os;
}

#endif /* OSTREAM_HELPER_H */

