#ifndef MATRIX_H
#define MATRIX_H

#include <cstdarg>

#include "InvalidDimensionException.h"
#include "Vector.h"

namespace engine {
    namespace math {
        template<int dimCols, int dimRows>
        class Matrix {
        protected:
            // Coords are (col, row) or (x, y).
            // Saved like this:
            // { (0, 0), (1, 0), (2, 0), ..., (0, 1), (1, 1), .... }
            float elements[dimRows * dimCols];
            
            inline void checkCoords(int x, int y) const {
                if(x < 0 || x >= dimCols || y < 0 || y >= dimRows) {
                    throw InvalidDimensionException("Coordinates exceed Matrix dimensions.");
                }
            }
            
            inline int coordToIndex(int x, int y) const {
                return x + y * dimRows;
            }
            
        public:
            Matrix() {}
            
            Matrix(float elems...) {
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
                std::memcpy(this->elements, orig.elements, dimCols * dimRows * sizeof(float));
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
                
                for(int y = 0; y < dimRows; ++y) {
                    for(int x = 0; x < dimRows; ++x) {
                        ret.elements[ret.coordToIndex(x, y)] = 0;
                        
                        for(int i = 0; i < dimCols; ++i) {
                            ret.elements[ret.coordToIndex(x, y)] += 
                                    this->elements[this->coordToIndex(i, y)]
                                    * m.elements[m.coordToIndex(x, i)];
                        }
                    }
                }
                
                return ret;
            }
            
            template<int subDimCols, int subDimRows>
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
        };
    }
}

#endif /* MATRIX_H */

