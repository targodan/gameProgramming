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
            std::size_t size;
            T* data = nullptr;
            
            inline void checkIndex(const std::size_t& i) {
#ifdef DEBUG
                if(i >= this->size) {
                    throw IndexOutOfBoundsException("Index %du is out of bounds. Array size is %du.", i, this->size);
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
                this->size = size;
                this->data = new T[this->size];
            }
            
            Array(const Array& orig) : Array(orig.size) {
                std::copy(orig.data, orig.data + orig.size, this->data);
            }
            
            Array(const std::initializer_list<T>& list) : Array(list.size()) {
                std::copy(list.begin(), list.end(), this->data);
            }
            
            Array(Array&& orig) {
                this->size = size;
                this->data = orig.data;
                orig.data = nullptr;
            }
            
            virtual ~Array() {
                if(this->data != nullptr) {
                    delete[] this->data;
                }
            }
            
            const T& operator[](std::size_t i) const {
                return this->data[i];
            }
            
            T& operator[](std::size_t i) {
                return this->data[i];
            }
            
            Iterator begin() {
                return Iterator(this, 0);
            }
            
            Iterator end() {
                return Iterator(this, this->size);
            }
            
            ConstIterator begin() const {
                return ConstIterator(this, 0);
            }
            
            ConstIterator end() const {
                return ConstIterator(this, this->size);
            }
            
            static void swap(Array& left, Array& right) {
                std::swap(left.size, right.size);
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

