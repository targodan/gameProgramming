#ifndef SET_H
#define SET_H

#include <sparsehash/dense_hash_set>
#include "hashes.h"

namespace engine {
    namespace util {
        template<class T>
        using Set = google::dense_hash_set<T, std::hash<T>>;
    }
}


#endif /* SET_H */

