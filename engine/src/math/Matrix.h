#ifndef MATRIX_H
#define MATRIX_H

#include <initializer_list>
#include <memory>
#include <sstream>

#include "InvalidDimensionException.h"
#include "Vector.h"
#include "Vector3.h"
#include "../util/Array.h"

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
            Array<float> elements;
            
            inline void checkCoords(unsigned int x, unsigned int y) const {
#ifdef DEBUG
                if(x < 0 || x >= dimCols || y < 0 || y >= dimRows) {
                    throw InvalidDimensionException("Coordinates (%du, %du) exceed Matrix dimensions.", x, y);
                }
#endif
            }
            
            inline unsigned int coordToIndex(unsigned int x, unsigned int y) const {
                return x + y * dimCols;
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
#ifdef DEBUG
                    if(y < 0 || y >= dimRows) {
                        throw InvalidDimensionException("y exceeds matrix rows.");
                    }
#endif
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
#ifdef DEBUG
                    if(y < 0 || y >= dimRows) {
                        throw InvalidDimensionException("y exceeds matrix rows.");
                    }
#endif
                    return mat->elements[mat->coordToIndex(this->x, y)];
                }
            };
            
        public:
            Matrix() : elements(dimRows * dimCols) {
                std::fill(this->elements.begin(), this->elements.end(), 0.);
            }
            
            Matrix(std::initializer_list<float> list) : elements(dimRows * dimCols) {
                if(list.size() < dimCols * dimRows) {
                    throw InvalidDimensionException("Not enough elements in list.");
                } else if(list.size() > dimCols * dimRows) {
                    throw InvalidDimensionException("Too many elements in list.");
                }
                std::copy(list.begin(), list.end(), this->elements.begin());
            }
            
            Matrix(const Matrix& orig) : elements(dimRows * dimCols) {
                std::copy(orig.elements.begin(), orig.elements.end(), this->elements.begin());
            }
            
            Matrix(Matrix&& orig) : elements(0) {
                Matrix::swap(*this, orig);
            }
            
            ~Matrix() {}
            
            Matrix& operator=(Matrix&& m) {
                Matrix::swap(*this, m);
                return *this;
            }
            
            Matrix<dimCols, dimRows>& operator=(std::initializer_list<float> list) {
                if(list.size() < dimCols * dimRows) {
                    throw InvalidDimensionException("Not enough elements in list.");
                } else if(list.size() > dimCols * dimRows) {
                    throw InvalidDimensionException("Too many elements in list.");
                }
                std::copy(list.begin(), list.end(), this->elements.begin());
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
            
            Matrix<dimCols, dimRows>& add(const Matrix<dimCols, dimRows>& m) {
                for(unsigned int y = 0; y < dimRows; ++y) {
                    for(unsigned int x = 0; x < dimCols; ++x) {
                        this->checkCoords(x, y);
                        this->elements[this->coordToIndex(x, y)] += 
                                m.elements[m.coordToIndex(x, y)];
                    }
                }
                
                return *this;
            }
            
            Matrix<dimCols, dimRows>& sub(const Matrix<dimCols, dimRows>& m) {
                for(unsigned int y = 0; y < dimRows; ++y) {
                    for(unsigned int x = 0; x < dimCols; ++x) {
                        this->elements[this->coordToIndex(x, y)] -= 
                                m.elements[m.coordToIndex(x, y)];
                    }
                }
                
                return *this;
            }
            
            Matrix<dimRows, dimCols> transpose() const {
                Matrix<dimRows, dimCols> ret;
                
                for(unsigned int y = 0; y < dimRows; ++y) {
                    for(unsigned int x = 0; x < dimCols; ++x) {
                        ret.elements[ret.coordToIndex(y, x)] = this->elements[this->coordToIndex(x, y)];
                    }
                }
                
                return ret;
            }
            
            inline unsigned int getDimensionRows() const {
                return dimRows;
            }
            
            inline unsigned int getDimensionCols() const {
                return dimCols;
            }
            
            template<unsigned int subDimCols, unsigned int subDimRows>
            Matrix<subDimCols, subDimRows> getSubMatrix(int xTopLeft, int yTopLeft) const {
#ifdef DEBUG
                if(subDimCols + xTopLeft >= dimCols || subDimRows + yTopLeft >= dimRows) {
                    throw InvalidDimensionException("The sub matrix is out of bounds.");
                }
#endif
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
#ifdef DEBUG
                if(x < 0 || x >= dimCols || y < 0 || y >= dimRows) {
                    throw InvalidDimensionException("The axis is out of bounds.");
                }
#endif
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
            
            bool operator==(const Matrix<dimCols, dimRows>& right) const {
                for(unsigned int y = 0; y < dimRows; ++y) {
                    for(unsigned int x = 0; x < dimCols; ++x) {
                        if(this->elements[this->coordToIndex(x, y)]
                                != right.elements[right.coordToIndex(x, y)]) {
                            return false;
                        }
                    }
                }
                return true;
            }
            
            bool operator!=(const Matrix<dimCols, dimRows>& right) const {
                for(unsigned int y = 0; y < dimRows; ++y) {
                    for(unsigned int x = 0; x < dimCols; ++x) {
                        if(this->elements[this->coordToIndex(x, y)]
                                == right.elements[right.coordToIndex(x, y)]) {
                            return false;
                        }
                    }
                }
                return true;
            }
            
            std::string toString() const {
                std::stringstream ss;
                for(unsigned int y = 0; y < dimRows; ++y) {
                    ss << "| ";
                    for(unsigned int x = 0; x < dimCols; ++x) {
                        ss << std::setw(8) << this->elements[this->coordToIndex(x, y)];
                        if(x+1 < dimCols) {
                            ss << " ";
                        }
                    }
                    ss << " |" << std::endl;
                }
                return ss.str();
            }
            
            friend std::ostream& operator<<(std::ostream& os, const Matrix<dimCols, dimRows>& m) {
                os << m.toString();
                return os;
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
            
            friend Matrix<dimCols, dimRows> operator+(
                    Matrix<dimCols, dimRows> m1,
                    const Matrix<dimCols, dimRows>& m2) {
                return m1.add(m2);
            }
            
            friend Matrix<dimCols, dimRows> operator-(
                    Matrix<dimCols, dimRows> m1,
                    const Matrix<dimCols, dimRows>& m2) {
                return m1.sub(m2);
            }
            
            friend Matrix<dimRows, dimRows> operator*(
                    Matrix<dimCols, dimRows> m,
                    const float& f) {
                return m.mul(f);
            }
            
            friend Matrix<dimRows, dimRows> operator*(
                    const float& f,
                    Matrix<dimCols, dimRows> m) {
                return m.mul(f);
            }
            
            friend Matrix<dimRows, dimRows> operator/(
                    Matrix<dimCols, dimRows> m,
                    const float& f) {
                return m.div(f);
            }
            
            Matrix<dimCols, dimRows>& operator+=(const Matrix<dimCols, dimRows>& m) {
                return this->add(m);
            }
            
            Matrix<dimCols, dimRows>& operator-=(const Matrix<dimCols, dimRows>& m) {
                return this->sub(m);
            }
            
            Matrix<dimCols, dimRows>& operator*=(const float& f) {
                return this->mul(f);
            }
            
            Matrix<dimCols, dimRows>& operator/=(const float& f) {
                return this->div(f);
            }
            
            // All matricies are freinds. Aaawwww :D
            template<unsigned int, unsigned int>
            friend class Matrix;
            
            friend class MatrixSq<dimRows>;
        };
    }
}

#endif /* MATRIX_H */

