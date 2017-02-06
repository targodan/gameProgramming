#ifndef VECTOR_H
#define VECTOR_H

#include <vector>
#include <initializer_list>

#include "IndexOutOfBoundsException.h"

namespace engine {
    namespace util {
        template<typename _Tp,typename _Alloc=std::allocator<_Tp>>
        class vector : public std::vector<_Tp, _Alloc> {
        private:
            inline void checkIndex(size_t i) const {
                if(i >= this->size()) {
                    throw IndexOutOfBoundsException("Index %zu out of bounds. Size %zu.", i, this->size());
                }
            }
            
        public:
            vector() noexcept : std::vector<_Tp, _Alloc>() {}
            vector(size_t size) noexcept : std::vector<_Tp, _Alloc>(size) {}
            vector(size_t size, const _Tp& value) noexcept : std::vector<_Tp, _Alloc>(size, value) {}
            vector(const vector& __x) noexcept : std::vector<_Tp, _Alloc>(__x) {}
            vector(vector&& __x) noexcept : std::vector<_Tp, _Alloc>(std::move(__x)) {}
            vector(std::initializer_list<_Tp> __l) noexcept : std::vector<_Tp, _Alloc>(__l) {}
            ~vector() noexcept {}
            
            vector& operator=(const vector& __x) {
                std::vector<_Tp, _Alloc>::operator=(__x);
                return *this;
            }
            vector& operator=(vector&& __x) {
                std::vector<_Tp, _Alloc>::operator=(std::move(__x));
                return *this;
            }
            vector& operator=(std::initializer_list<_Tp> __l) {
                std::vector<_Tp, _Alloc>::operator=(__l);
                return *this;
            }
            
#ifdef DEBUG
            _Tp& operator[](size_t i) {
                this->checkIndex(i);
                return std::vector<_Tp, _Alloc>::operator[](i);
            }
            const _Tp& operator[](size_t i) const {
                this->checkIndex(i);
                return std::vector<_Tp, _Alloc>::operator[](i);
            }
            _Tp& at(size_t i) {
                this->checkIndex(i);
                return std::vector<_Tp, _Alloc>::at(i);
            }
            const _Tp& at(size_t i) const {
                this->checkIndex(i);
                return std::vector<_Tp, _Alloc>::at(i);
            }
#endif
        };
    }
}

#endif /* VECTOR_H */

