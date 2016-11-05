#ifndef HASHES_H
#define	HASHES_H

namespace engine {
    namespace util {
        template<typename key_t>
        class __PrimeHash {
        public:
            size_t operator()(const key_t& key) const {
                const unsigned char* data = reinterpret_cast<const unsigned char*>(&key);
                size_t hash = 7;
                for(size_t i = 0; i < sizeof(key_t); ++i) {
                    hash = hash*31 + data[i];
                }
                return hash;
            }
        };

        template<>
        class __PrimeHash<std::string> {
        public:
            size_t operator()(const std::string& key) const {
                size_t hash = 7;
                for(size_t i = 0; i < key.length(); ++i) {
                    hash = hash*31 + key[i];
                }
                return hash;
            }
        };

        template<>
        class __PrimeHash<std::string*> {
        public:
            size_t operator()(const std::string*& key) const {
                size_t hash = 7;
                for(size_t i = 0; i < key->length(); ++i) {
                    hash = hash*31 + (*key)[i];
                }
                return hash;
            }
        };

        template<typename key_t>
        class __XorHash {
        public:
            size_t operator()(const key_t& key) const {
                const char* data = reinterpret_cast<const char*>(&key);
                size_t hash = 0;
                unsigned char* hash_b = reinterpret_cast<unsigned char*>(&hash);
                for(size_t i = 0; i < sizeof(key_t); ++i) {
                    hash_b[i % sizeof(size_t)] ^= data[i];
                }
                return hash;
            }
        };

        template<>
        class __XorHash<std::string> {
        public:
            size_t operator()(const std::string& key) const {
                size_t hash = 0;
                unsigned char* hash_b = reinterpret_cast<unsigned char*>(&hash);
                for(size_t i = 0; i < key.length(); ++i) {
                    hash_b[i % sizeof(size_t)] ^= key[i];
                }
                return hash;
            }
        };

        template<>
        class __XorHash<std::string*> {
        public:
            size_t operator()(const std::string*& key) const {
                size_t hash = 0;
                unsigned char* hash_b = reinterpret_cast<unsigned char*>(&hash);
                for(size_t i = 0; i < key->length(); ++i) {
                    hash_b[i % sizeof(size_t)] ^= (*key)[i];
                }
                return hash;
            }
        };

        template<typename key_t, int hashSize>
        class __FNVHash {
        public:
            size_t operator()(const key_t& key) const {
                // by default expect 64 bits
                const unsigned char* data = reinterpret_cast<const unsigned char*>(&key);
                size_t hash = 14695981039346656037U;
                for(size_t i = 0; i < sizeof(key); ++i) {
                    hash = (hash ^ data[i]) * 1099511628211U;
                }
                return hash;
            }
        };

        template<typename key_t>
        class __FNVHash<key_t, 4> {
        public:
            size_t operator()(const key_t& key) const {
                // by default expect 64 bits
                const unsigned char* data = reinterpret_cast<const unsigned char*>(&key);
                size_t hash = 2166136261U;
                for(size_t i = 0; i < sizeof(key); ++i) {
                    hash = (hash ^ data[i]) * 16777619U;
                }
                return hash;
            }
        };

        template<int hashSize>
        class __FNVHash<std::string, hashSize> {
        public:
            size_t operator()(const std::string& key) const {
                size_t hash = 14695981039346656037U;
                for(size_t i = 0; i < key.length(); ++i) {
                    hash = (hash ^ key.at(i)) * 1099511628211U;
                }
                return hash;
            }
        };

        template<int hashSize>
        class __FNVHash<std::string*, hashSize> {
        public:
            size_t operator()(const std::string*& key) const {
                size_t hash = 14695981039346656037U;
                for(size_t i = 0; i < key->length(); ++i) {
                    hash = (hash ^ key->at(i)) * 1099511628211U;
                }
                return hash;
            }
        };

        template<>
        class __FNVHash<std::string, 4> {
        public:
            size_t operator()(const std::string& key) const {
                size_t hash = 2166136261U;
                for(size_t i = 0; i < key.length(); ++i) {
                    hash = (hash ^ key.at(i)) * 16777619U;
                }
                return hash;
            }
        };

        template<>
        class __FNVHash<std::string*, 4> {
        public:
            size_t operator()(const std::string*& key) const {
                size_t hash = 2166136261U;
                for(size_t i = 0; i < key->length(); ++i) {
                    hash = (hash ^ key->at(i)) * 16777619U;
                }
                return hash;
            }
        };
    }
}

#endif	/* HASHES_H */

