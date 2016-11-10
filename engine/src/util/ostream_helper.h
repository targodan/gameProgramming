#ifndef OSTREAM_HELPER_H
#define OSTREAM_HELPER_H

#include <iostream>
#include <vector>
#include <type_traits>
#include <memory>

namespace std {
    template <typename T> 
    struct is_pointer<shared_ptr<T>> : std::true_type {};
    template <typename T> 
    struct is_pointer<unique_ptr<T>> : std::true_type {};
    template <typename T> 
    struct is_pointer<weak_ptr<T>> : std::true_type {};
}

template<class T, typename std::enable_if<std::is_pointer<T>::value, bool>::type = 0>
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

template<class T, typename std::enable_if<!std::is_pointer<T>::value, bool>::type = 0>
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

