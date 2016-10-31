#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <cstring>
#include <initializer_list>

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
            float* elements;
            
            static void swap(Vector<dimension>& v1, Vector<dimension>& v2) {
                std::swap(v1.elements, v2.elements);
            }

        public:
            Vector() {
                this->elements = new float[dimension];
                std::fill(this->elements, this->elements + dimension, 0.);
            }
            
            Vector(std::initializer_list<float> list) : Vector() {
                if(list.size() < dimension) {
                    throw InvalidDimensionException("Not enough elements in list.");
                } else if(list.size() > dimension) {
                    throw InvalidDimensionException("Too many elements in list.");
                }
                std::memcpy(this->elements, list.begin(), dimension * sizeof(float));
            }

            Vector(const Vector& orig) {
                this->elements = new float[dimension];
                std::memcpy(this->elements, orig.elements, dimension * sizeof(float));
            }

            Vector(Vector<dimension>&& orig) : Vector<dimension>() {
                Vector<dimension>::swap(*this, orig);
            }

            ~Vector() {
                delete[] this->elements;
            }
            
            Vector<dimension>& operator=(Vector<dimension>&& v) {
                Vector<dimension>::swap(*this, v);
                return *this;
            }
            
            Vector<dimension>& operator=(std::initializer_list<float> list) {
                if(list.size() < dimension) {
                    throw InvalidDimensionException("Not enough elements in list.");
                } else if(list.size() > dimension) {
                    throw InvalidDimensionException("Too many elements in list.");
                }
                std::memcpy(this->elements, list.begin(), dimension * sizeof(float));
            }

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
            
            template<unsigned int, unsigned int>
            friend class Matrix;
            
            friend class Vector3;
        };
    }
}

#endif /* VECTOR_H */

