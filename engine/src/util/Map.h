#ifndef MAP_H
#define	MAP_H

#include <algorithm>
#include <stddef.h>
#include <utility>
#include <stdexcept>

#include <string>

#include "hashes.h"

#define DBC_MAP_GROWTH_RATE 1.5
#define DBC_MAP_GROWTH_THRESH 0.5

namespace engine {
    namespace util {
        template<typename key_t, typename data_t, typename hash_t = __FNVHash<key_t, sizeof(size_t)> >
        class Map {
        public:
            class const_iterator {
            public:
                const_iterator(const Map<key_t,data_t,hash_t>* map, size_t index) : m_map(map), m_index(index) {}

                bool operator==(const const_iterator& it) const {
                    return m_map == it.m_map && m_index == it.m_index;
                }
                bool operator!=(const const_iterator& it) const {
                    return !(*this == it);
                }

                const std::pair<key_t, data_t>& operator*() const {
                    size_t index = m_map->findIndex(m_map->m_keys[m_index]);
                    if(m_map->m_container[index] == nullptr) {
                        throw std::out_of_range("This should not happen!");
                    }
                    return *(m_map->m_container[index]);
                }

                const std::pair<key_t, data_t>* operator->() const {
                    size_t index = m_map->findIndex(m_map->m_keys[m_index]);
                    if(m_map->m_container[index] == nullptr) {
                        throw std::out_of_range("This should not happen!");
                    }
                    return m_map->m_container[index];
                }

                const_iterator& operator++() {
                    ++m_index;
                    return *this;
                }

                const_iterator operator++(int) {
                    const_iterator ret(m_map, m_index);
                    return ++ret;
                }
            private:
                const Map<key_t,data_t,hash_t>* m_map;
                size_t m_index;
            };

            class iterator {
            public:
                iterator(Map<key_t,data_t,hash_t>* map, size_t index) : m_map(map), m_index(index) {}

                operator const_iterator() {
                    return const_iterator(m_map, m_index);
                }

                bool operator==(const iterator& it) const {
                    return m_map == it.m_map && m_index == it.m_index;
                }
                bool operator!=(const iterator& it) const {
                    return !(*this == it);
                }

                std::pair<key_t, data_t>& operator*() {
                    size_t index = m_map->findIndex(m_map->m_keys[m_index]);
                    if(m_map->m_container[index] == nullptr) {
                        throw std::out_of_range("This should not happen!");
                    }
                    return *(m_map->m_container[index]);
                }

                std::pair<key_t, data_t>* operator->() {
                    size_t index = m_map->findIndex(m_map->m_keys[m_index]);
                    if(m_map->m_container[index] == nullptr) {
                        throw std::out_of_range("This should not happen!");
                    }
                    return m_map->m_container[index];
                }

                iterator& operator++() {
                    ++m_index;
                    return *this;
                }

                iterator operator++(int) {
                    iterator ret(m_map, m_index);
                    return ++ret;
                }
            private:
                Map<key_t,data_t,hash_t>* m_map;
                size_t m_index;
            };

            Map(size_t initialCapacity = 205) : m_capacity(initialCapacity), m_size(0) {
                if(m_capacity % 2 == 0) {
                    ++m_capacity;
                }
                m_keys = new key_t[static_cast<size_t>(m_capacity * DBC_MAP_GROWTH_THRESH)];
                m_container = new std::pair<key_t,data_t>*[m_capacity];
                std::fill_n(m_container, m_capacity, static_cast<std::pair<key_t,data_t>*>(nullptr));
            }
            Map(const Map<key_t,data_t,hash_t>& orig) {}
            ~Map() {
                clear();
                delete[] m_keys;
                delete[] m_container;
            }

            data_t& operator[](const key_t& key) {
                size_t index = findIndex(key);
                if(m_container[index] == nullptr) {
                    std::pair<key_t, data_t>* tmp = new std::pair<key_t, data_t>(key, nullptr);
                    index = put(tmp);
                    if(m_size > static_cast<size_t>(DBC_MAP_GROWTH_THRESH * m_capacity)) {
                        grow();
                        index = findIndex(key);
                    }
                }
                return m_container[index]->second;
            }

            const data_t& operator[](const key_t& key) const {
                size_t index = findIndex(key);
                if(m_container[index] == nullptr) {
                    throw std::out_of_range("That key does not exist!");
                }
                return m_container[index]->second;
            }

            data_t& at(const key_t& key) {
                size_t index = findIndex(key);
                if(m_container[index] == nullptr) {
                    throw std::out_of_range("That key does not exist!");
                }
                return m_container[index]->second;
            }

            const data_t& at(const key_t& key) const {
                size_t index = findIndex(key);
                if(m_container[index] == nullptr) {
                    throw std::out_of_range("That key does not exist!");
                }
                return m_container[index]->second;
            }

            void push(key_t& key, data_t& data) {
                put(new std::pair<key_t, data_t>(key, data));
                if(m_size > static_cast<size_t>(DBC_MAP_GROWTH_THRESH * m_capacity)) {
                    grow();
                }
            }

            const_iterator begin() const {
                return const_iterator(this, 0);
            }

            const_iterator find(const key_t& key) const {
                size_t index = findIndex(key);
                if(m_container[index] == nullptr) {
                    return this->end();
                } else {
                    return const_iterator(this, index);
                }
            }

            const_iterator end() const {
                return const_iterator(this, m_size);
            }

            iterator begin() {
                return iterator(this, 0);
            }

            iterator find(const key_t& key) {
                size_t index = findIndex(key);
                if(m_container[index] == nullptr) {
                    return this->end();
                } else {
                    return iterator(this, index);
                }
            }

            iterator end() {
                return iterator(this, m_size);
            }

            size_t size() const {
                return m_size;
            }

            void clear() {
                for(size_t i = 0; i < m_size; ++i) {
                    const key_t& key = m_keys[i];
                    size_t index = findIndex(key);
                    delete m_container[index];
                    m_container[index] = nullptr;
                }
                m_size = 0;
            }
        private:
            size_t indexCollision(size_t index) const {
                return indexCollision(index, m_capacity);
            }
            size_t indexCollision(size_t index, size_t capacity) const {
                return (index + capacity / 2) % capacity;
            }

            size_t findIndex(const key_t& key) const {
                return findIndex(key, m_container, m_capacity);
            }
            size_t findIndex(const key_t& key, std::pair<key_t,data_t>** container, size_t capacity) const {
                size_t index = this->m_hasher(key) % capacity;
                while(container[index] != nullptr && container[index]->first != key) {
                    index = indexCollision(index);
                }
                return index;
            }

            size_t put(std::pair<key_t,data_t>* data) {
                m_keys[m_size] = data->first;
                size_t index = findIndex(data->first);
                if(m_container[index] != nullptr) {
                    delete m_container[index];
                }
                m_container[index] = data;
                ++m_size;
                return index;
            }

            void grow() {
                size_t oldCap = m_capacity;
                m_capacity = static_cast<size_t>(DBC_MAP_GROWTH_RATE * m_capacity);
                key_t* oldKeys = m_keys;
                m_keys = new key_t[static_cast<size_t>(m_capacity * DBC_MAP_GROWTH_THRESH)];
                std::pair<key_t,data_t>** oldContainer = m_container;
                m_container = new std::pair<key_t,data_t>*[m_capacity];
                std::fill_n(m_container, m_capacity, static_cast<std::pair<key_t,data_t>*>(nullptr));

                size_t oldSize = m_size;
                m_size = 0;
                for(size_t i = 0; i < oldSize; ++i) {
                    const key_t& key = oldKeys[i];
                    std::pair<key_t,data_t>* data = oldContainer[findIndex(key, oldContainer, oldCap)];
                    put(data);
                }
                delete[] oldKeys;
                delete[] oldContainer;
            }

            size_t m_capacity;
            size_t m_size;
            key_t* m_keys;
            std::pair<key_t,data_t>** m_container;
            hash_t m_hasher;
        };
    }
}

#endif	/* MAP_H */

