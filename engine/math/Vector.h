#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <cstdarg>

#include "functions.h"
#include "InvalidDimensionException.h"

namespace engine {
    namespace math {
        template<int dimension>
        class Vector {
        protected:
            float elements[dimension];
            
            void swap(Vector<dimension>& v) {
                std::swap(this->elements, v.elements);
            }

        public:
            Vector() {}
            
            Vector(float elems...) {
                va_list args;
                va_start(args, elems);

                for(int i = 0; i < dimension; ++i) {
                    this->elements[i] = va_arg(args, int);
                }

                va_end(args);
            }

            Vector(const Vector& orig) {
                for(int i = 0; i < dimension; ++i) {
                    this->elements[i] = orig.elements[i];
                }
                std::cout << "COPY" << std::endl;
            }

            ~Vector() {}

            Vector<dimension>& add(const Vector<dimension>& v) {
                for(int i = 0; i < dimension; ++i) {
                    this->elements[i] += v.elements[i];
                }
                return *this;
            }

            Vector<dimension>& sub(const Vector<dimension>& v) {
                for(int i = 0; i < dimension; ++i) {
                    this->elements[i] -= v.elements[i];
                }
                return *this;
            }

            Vector<dimension>& add(const float& v) {
                for(int i = 0; i < dimension; ++i) {
                    this->elements[i] += v;
                }
                return *this;
            }

            Vector<dimension>& sub(const float& v) {
                for(int i = 0; i < dimension; ++i) {
                    this->elements[i] -= v;
                }
                return *this;
            }

            Vector<dimension>& mul(const float& v) {
                for(int i = 0; i < dimension; ++i) {
                    this->elements[i] *= v;
                }
                return *this;
            }

            Vector<dimension>& div(const float& v) {
                for(int i = 0; i < dimension; ++i) {
                    this->elements[i] /= v;
                }
                return *this;
            }
            
            Vector<dimension>& normalize() {
                return this->mul(this->length());
            }
            
            float lengthSquare() const {
                float ret = 0;
                for(int i = 0; i < dimension; ++i) {
                    ret += this->elements[i] * this->elements[i];
                }
                return ret;
            }
            
            float length() const {
                return sqrt(this->lengthSquare());
            }
            
            float operator[](int i) const {
                if(i >= dimension) {
                    throw InvalidDimensionException(
                            "Tried to access an element that does not exist."
                        );
                }
                return this->elements[i];
            }
            
            float& operator[](int i) {
                if(i >= dimension) {
                    throw InvalidDimensionException(
                            "Tried to access an element that does not exist."
                        );
                }
                return this->elements[i];
            }
            
            Vector<dimension>& operator+=(const Vector<dimension>& v) {
                return this->add(v);
            }
            
            Vector<dimension>& operator-=(const Vector<dimension>& v) {
                return this->sub(v);
            }
            
            Vector<dimension>& operator+=(const float& v) {
                return this->add(v);
            }
            
            Vector<dimension>& operator-=(const float& v) {
                return this->sub(v);
            }
            
            Vector<dimension>& operator*=(const float& v) {
                return this->mul(v);
            }
            
            Vector<dimension>& operator/=(const float& v) {
                return this->div(v);
            }
            
            friend Vector<dimension> operator+(Vector<dimension> v1, const Vector<dimension>& v2) {
                return v1.add(v2);
            }
            
            friend Vector<dimension> operator-(Vector<dimension> v1, const Vector<dimension>& v2) {
                return v1.sub(v2);
            }
            
            friend Vector<dimension> operator+(Vector<dimension> v, const float& f) {
                return v.add(f);
            }
            
            friend Vector<dimension> operator-(Vector<dimension> v, const float& f) {
                return v.sub(f);
            }
            
            friend Vector<dimension> operator*(Vector<dimension> v, const float& f) {
                return v.mul(f);
            }
            
            friend Vector<dimension> operator/(Vector<dimension> v, const float& f) {
                return v.div(f);
            }
            
            friend Vector<dimension> operator+(const float& f, Vector<dimension> v) {
                return v.add(f);
            }
            
            friend Vector<dimension> operator-(const float& f, Vector<dimension> v) {
                return v.sub(f);
            }
            
            friend Vector<dimension> operator*(const float& f, Vector<dimension> v) {
                return v.mul(f);
            }
            
            friend Vector<dimension> operator/(const float& f, Vector<dimension> v) {
                return v.div(f);
            }
            
            Vector<dimension>& operator=(Vector<dimension> v) {
                swap(v);
                return *this;
            }
        };
    }
}

#endif /* VECTOR_H */

