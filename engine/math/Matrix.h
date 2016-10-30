#ifndef MATRIX_H
#define MATRIX_H

#include <cstdarg>

#include "InvalidDimensionException.h"
#include "Vector.h"
#include "Vector3.h"

namespace engine {
    namespace math {
        template<unsigned int dimension>
        class MatrixSq;
            
        template<unsigned int dimCols, unsigned int dimRows>
        class Matrix {
        protected:
            // Coords are (col, row) or (x, y).
            // Saved like this:
            // { (0, 0), (1, 0), (2, 0), ..., (0, 1), (1, 1), .... }
            float* elements;
            
            inline void checkCoords(unsigned int x, unsigned int y) const {
                if(x < 0 || x >= dimCols || y < 0 || y >= dimRows) {
                    throw InvalidDimensionException("Coordinates exceed Matrix dimensions.");
                }
            }
            
            inline unsigned int coordToIndex(unsigned int x, unsigned int y) const {
                return x + y * dimRows;
            }
            
            static void swap(Matrix& m1, Matrix& m2) {
                std::swap(m1.elements, m2.elements);
            }
            
            class Proxy {
            protected:
                Matrix* mat;
                unsigned int x;
                
            public:
                Proxy(Matrix* mat, unsigned int x) : mat(mat), x(x) {}
                
                float& operator[](unsigned int y) {
                    if(y < 0 || y >= dimRows) {
                        throw InvalidDimensionException("y exceeds matrix rows.");
                    }
                    return mat->elements[mat->coordToIndex(this->x, y)];
                }
            };
            
            class ConstProxy {
            protected:
                const Matrix* mat;
                int x;
                
            public:
                ConstProxy(const Matrix* mat, int x) : mat(mat), x(x) {}
                
                float operator[](int y) const {
                    if(y < 0 || y >= dimRows) {
                        throw InvalidDimensionException("y exceeds matrix rows.");
                    }
                    return mat->elements[mat->coordToIndex(this->x, y)];
                }
            };
            
        public:
            Matrix() {
                this->elements = new float[dimRows * dimCols];
                std::fill(this->elements, this->elements + (dimCols * dimRows), 0.);
            }
            
            Matrix(float elems...) {
                this->elements = new float[dimRows * dimCols];
                
                va_list args;
                va_start(args, elems);

                for(int y = 0; y < dimRows; ++y) {
                    for(int x = 0; x < dimCols; ++x) {
                        this->elements[this->coordToIndex(x, y)] = static_cast<float>(va_arg(args, double));
                    }
                }

                va_end(args);
            }
            
            Matrix(const Matrix& orig) {
                this->elements = new float[dimRows * dimCols];
                std::memcpy(this->elements, orig.elements, dimCols * dimRows * sizeof(float));
            }
            
            Matrix(Matrix&& orig) : Matrix() {
                Matrix::swap(*this, orig);
            }
            
            ~Matrix() {
                delete[] this->elements;
            }
            
            Matrix& operator=(Matrix&& m) {
                Matrix::swap(*this, m);
                return *this;
            }
            
            Vector<dimRows> mul(const Vector<dimCols>& v) const {
                Vector<dimRows> ret;
                
                for(int y = 0; y < dimRows; ++y) {
                    ret.elements[y] = 0;
                    for(int x = 0; x < dimCols; ++x) {
                        ret.elements[y] += this->elements[this->coordToIndex(x, y)] * v.elements[x];
                    }
                }
                
                return ret;
            }
            
            Matrix<dimCols, dimRows>& mul(const float& v) {
                for(int y = 0; y < dimRows; ++y) {
                    for(int x = 0; x < dimCols; ++x) {
                        this->elements[this->coordToIndex(x, y)] *= v;
                    }
                }
                return *this;
            }
            
            Matrix<dimCols, dimRows>& div(const float& v) {
                for(int y = 0; y < dimRows; ++y) {
                    for(int x = 0; x < dimCols; ++x) {
                        this->elements[this->coordToIndex(x, y)] /= v;
                    }
                }
                return *this;
            }
            
            Matrix<dimRows, dimRows> mul(const Matrix<dimRows, dimCols>& m) const {
                Matrix<dimRows, dimRows> ret;
                
                for(unsigned int y = 0; y < dimRows; ++y) {
                    for(unsigned int x = 0; x < dimRows; ++x) {
                        ret.elements[ret.coordToIndex(x, y)] = 0;
                        
                        for(unsigned int i = 0; i < dimCols; ++i) {
                            ret.elements[ret.coordToIndex(x, y)] += 
                                    this->elements[this->coordToIndex(i, y)]
                                    * m.elements[m.coordToIndex(x, i)];
                        }
                    }
                }
                
                return ret;
            }
            
            Matrix<dimRows, dimCols> transpose() const {
                Matrix<dimRows, dimCols> ret;
                
                for(int y = 0; y < dimRows; ++y) {
                    for(int x = 0; x < dimCols; ++x) {
                        ret.elements[ret.coordToIndex(y, x)] = this->elements[this->coordToIndex(x, y)];
                    }
                }
                
                return ret;
            }
            
            template<unsigned int subDimCols, unsigned int subDimRows>
            Matrix<subDimCols, subDimRows> getSubMatrix(int xTopLeft, int yTopLeft) const {
                if(subDimCols + xTopLeft >= dimCols || subDimRows + yTopLeft >= dimRows) {
                    throw InvalidDimensionException("The sub matrix is out of bounds.");
                }
                Matrix<subDimCols, subDimRows> ret;
                for(int y = 0; y < subDimRows; ++y) {
                    std::memcpy(
                            ret.elements + ret.coordToIndex(0, y),
                            this->elements + this->coordToIndex(xTopLeft, yTopLeft + y),
                            subDimCols * sizeof(float)
                        );
                }
                return ret;
            }
            
            Matrix<dimCols-1, dimRows-1> getSubMatrixWithoutAxis(int x, int y) const {
                if(x < 0 || x >= dimCols || y < 0 || y >= dimRows) {
                    throw InvalidDimensionException("The axis is out of bounds.");
                }
                if(x == 0 && y == 0) {
                    return this->getSubMatrix<dimCols-1, dimRows-1>(1, 1);
                } else if(x == 0 && y == dimRows-1) {
                    return this->getSubMatrix<dimCols-1, dimRows-1>(1, 0);
                } else if(x == dimCols-1 && y == 0) {
                    return this->getSubMatrix<dimCols-1, dimRows-1>(0, 1);
                } else if(x == dimCols-1 && y == dimRows-1) {
                    return this->getSubMatrix<dimCols-1, dimRows-1>(0, 0);
                }
                
                Matrix<dimCols-1, dimRows-1> ret;
                int yDest = 0;
                for(int ySrc = 0; ySrc < dimRows; ++ySrc) {
                    if(ySrc == y) {
                        continue;
                    }
                    std::memcpy(
                            ret.elements + ret.coordToIndex(0, yDest),
                            this->elements + this->coordToIndex(0, ySrc),
                            x * sizeof(float)
                        );
                    std::memcpy(
                            ret.elements + ret.coordToIndex(x, yDest),
                            this->elements + this->coordToIndex(x+1, ySrc),
                            (dimCols - 1 - x) * sizeof(float)
                        );
                    ++yDest;
                }
                return ret;
            }
            
            Proxy operator[](unsigned int x) {
                if(x < 0 || x >= dimCols) {
                    throw InvalidDimensionException("x exceeds matrix columns.");
                }
                return Proxy(this, x);
            }
            
            const ConstProxy operator[](unsigned int x) const {
                if(x < 0 || x >= dimCols) {
                    throw InvalidDimensionException("x exceeds matrix columns.");
                }
                return ConstProxy(this, x);
            }
            
            friend Matrix<dimRows, dimRows> operator*(
                    const Matrix<dimCols, dimRows>& m1,
                    const Matrix<dimRows, dimCols>& m2) {
                return m1.mul(m2);
            }
            
            friend Vector<dimRows> operator*(
                    const Matrix<dimCols, dimRows>& m,
                    const Vector<dimRows>& v) {
                return m.mul(v);
            }
            
            friend class MatrixSq<dimCols>;
        };
    }
}

#endif /* MATRIX_H */

