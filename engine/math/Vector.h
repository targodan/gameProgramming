#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <cstdarg>
#include <cstring>

#include "functions.h"
#include "InvalidDimensionException.h"

namespace engine {
    namespace math {
        template<unsigned int dimCols, unsigned int dimRows>
        class Matrix;
            
        class Vector3;
        
        template<unsigned int dimension>
        class Vector {
        protected:
            float elements[dimension];

        public:
            Vector() {
                std::fill(this->elements, this->elements + dimension, 0.);
            }
            
            Vector(float elems...) {
                va_list args;
                va_start(args, elems);

                for(unsigned int i = 0; i < dimension; ++i) {
                    this->elements[i] = static_cast<float>(va_arg(args, double));
                }

                va_end(args);
            }

            Vector(const Vector& orig) {
                std::memcpy(this->elements, orig.elements, dimension * sizeof(float));
            }

            ~Vector() {}

            Vector<dimension>& add(const Vector<dimension>& v) {
                for(unsigned int i = 0; i < dimension; ++i) {
                    this->elements[i] += v.elements[i];
                }
                return *this;
            }

            Vector<dimension>& sub(const Vector<dimension>& v) {
                for(unsigned int i = 0; i < dimension; ++i) {
                    this->elements[i] -= v.elements[i];
                }
                return *this;
            }

            Vector<dimension>& add(const float& v) {
                for(unsigned int i = 0; i < dimension; ++i) {
                    this->elements[i] += v;
                }
                return *this;
            }

            Vector<dimension>& sub(const float& v) {
                for(unsigned int i = 0; i < dimension; ++i) {
                    this->elements[i] -= v;
                }
                return *this;
            }

            Vector<dimension>& mul(const float& v) {
                for(unsigned int i = 0; i < dimension; ++i) {
                    this->elements[i] *= v;
                }
                return *this;
            }

            Vector<dimension>& div(const float& v) {
                for(unsigned int i = 0; i < dimension; ++i) {
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
            
            float operator[](unsigned int i) const {
                if(i < 0 || i >= dimension) {
                    throw InvalidDimensionException(
                            "Tried to access an element that does not exist."
                        );
                }
                return this->elements[i];
            }
            
            float& operator[](unsigned int i) {
                if(i < 0 || i >= dimension) {
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
                std::memcpy(this->elements, v.elements, dimension * sizeof(float));
                return *this;
            }
            
            template<unsigned int, unsigned int>
            friend class Matrix;
            
            friend class Vector3;
        };
    }
}

#endif /* VECTOR_H */

