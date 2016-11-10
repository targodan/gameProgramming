#ifndef MAP_H
#define MAP_H

#include <sparsehash/dense_hash_map>
#include "hashes.h"

namespace engine {
    namespace util {
        template<class Key, class T>
        using Map = google::dense_hash_map<Key, T, std::hash<Key>>;
    }
}


#endif /* MAP_H */

