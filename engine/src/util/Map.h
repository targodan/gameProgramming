#ifndef MAP_H
#define MAP_H

#include <sparsehash/dense_hash_map>
#include "hashes.h"

namespace engine {
    namespace util {
        template<class Key, class T, typename Hash = std::hash<Key>>
        using Map = google::dense_hash_map<Key, T, Hash>;
    }
}


#endif /* MAP_H */

