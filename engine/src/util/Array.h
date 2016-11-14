#ifndef ARRAY_H
#define ARRAY_H

#include <cstdlib>
#include <iterator>

#include "IndexOutOfBoundsException.h"

namespace engine {
    namespace util {
        template<class T>
        class Array {
        protected:
            std::size_t m_size;
            T* data = nullptr;
            
            inline void checkIndex(const std::size_t& i) const {
#ifdef DEBUG
                if(i >= this->m_size) {
                    throw IndexOutOfBoundsException("Index %du is out of bounds. Array size is %du.", i, this->m_size);
                }
#endif
            }
            
            class Iterator : public std::iterator<std::input_iterator_tag, T> {
            private:
                Array* arr;
                std::size_t index;
            public:
                Iterator(Array* arr, std::size_t index) : arr(arr), index(index) {}
                Iterator(const Iterator& orig) : arr(orig.arr), index(orig.index) {}
                
                Iterator& operator=(const Iterator& orig) {
                    this->arr = orig.arr;
                    this->index = orig.index;
                    return *this;
                }
                
                T& operator*() {
                    return this->arr->data[this->index];
                }
                
                Iterator& operator++() {
                    ++this->index;
                    return *this;
                }

                Iterator operator++(int) {
                    Iterator result(*this);
                    operator++();
                    return result;
                }
                
                bool operator!=(const Iterator& right) const {
                    bool result = !(*this == right); // Reuse equals operator
                    return result;
                }

                bool operator==(const Iterator& right) const {
                    return this->arr == right.arr && this->index == right.index;
                }
            };
            
            class ConstIterator : public std::iterator<std::output_iterator_tag, T> {
            private:
                const Array* arr;
                std::size_t index;
            public:
                ConstIterator(const Array* arr, std::size_t index) : arr(arr), index(index) {}
                ConstIterator(const ConstIterator& orig) : arr(orig.arr), index(orig.index) {}
                
                ConstIterator& operator=(const ConstIterator& orig) {
                    this->arr = orig.arr;
                    this->index = orig.index;
                    return *this;
                }
                
                const T& operator*() const {
                    return this->arr->data[this->index];
                }
                
                ConstIterator& operator++() {
                    ++this->index;
                    return *this;
                }

                ConstIterator operator++(int) {
                    Iterator result(*this);
                    operator++();
                    return result;
                }
                
                bool operator!=(const ConstIterator& right) const {
                    bool result = !(*this == right); // Reuse equals operator
                    return result;
                }

                bool operator==(const ConstIterator& right) const {
                    return this->arr == right.arr && this->index == right.index;
                }
            };
            
        public:
            Array(std::size_t size) {
                this->m_size = size;
                this->data = new T[this->m_size]();
            }
            
            Array(const Array& orig) : Array(orig.m_size) {
                std::copy(orig.data, orig.data + orig.m_size, this->data);
            }
            
            Array(const std::initializer_list<T>& list) : Array(list.size()) {
                std::copy(list.begin(), list.end(), this->data);
            }
            
            Array(Array&& orig) {
                this->swap(orig);
            }
            
            virtual ~Array() {
                if(this->data != nullptr) {
                    delete[] this->data;
                }
            }
            
            void swap(Array& orig) {
                std::swap(this->m_size, orig.m_size);
                std::swap(this->data, orig.data);
            }
            
            Array& operator=(const Array& orig) {
                this->m_size = orig.size();
                std::copy(orig.data, orig.data + orig.m_size, this->data);
                return *this;
            }
            
            Array& operator=(Array&& orig) {
                this->swap(orig);
                return *this;
            }
            
            const T& operator[](std::size_t i) const {
                this->checkIndex(i);
                return this->data[i];
            }
            
            T& operator[](std::size_t i) {
                this->checkIndex(i);
                return this->data[i];
            }
            
            Iterator begin() {
                return Iterator(this, 0);
            }
            
            Iterator end() {
                return Iterator(this, this->m_size);
            }
            
            ConstIterator begin() const {
                return ConstIterator(this, 0);
            }
            
            ConstIterator end() const {
                return ConstIterator(this, this->m_size);
            }
            
            void resize(size_t newSize) {
                T* newPtr = new T[newSize];
                std::move(this->data, this->data+this->m_size, newPtr);
                delete[] this->data;
                
                this->data = newPtr;
                this->m_size = newSize;
            }
                
            std::size_t size() const {
                return this->m_size;
            }
            
            static void swap(Array& left, Array& right) {
                std::swap(left.m_size, right.m_size);
                std::swap(left.data, right.data);
            }
        };
    }
}

namespace std {
    template<class T>
    void swap(engine::util::Array<T>& left, engine::util::Array<T>& right) {
        engine::util::Array<T>::swap(left, right);
    }
}

#endif /* ARRAY_H */

