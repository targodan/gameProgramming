#ifndef HELPER_H
#define HELPER_H

#include <functional>
#include <algorithm>
#include "util/vector.h"

template<class in_value_t, class out_value_t, class out_container_t, class in_container_t>
out_container_t map_to(in_container_t what, std::function<out_value_t(in_value_t)> how) {
    out_container_t ret;
    for(const auto& in : what) {
        ret.push_back(how(in));
    }
    return ret;
}

#endif /* HELPER_H */

