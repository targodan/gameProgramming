#ifndef MATRIX_H
#define MATRIX_H

#include <cstdarg>

#include "InvalidDimensionException.h"

namespace engine {
    namespace math {
        template<int dimCols, int dimRows>
        class Matrix {
        protected:
            // Coords are (col, row) or (x, y).
            // Saved like this:
            // { (0, 0), (1, 0), (2, 0), ..., (0, 1), (1, 1), .... }
            float elements[dimRows * dimCols];
            
            inline void checkCoords(int x, int y) {
                if(x < 0 || x >= dimCols || y < 0 || y >= dimRows) {
                    throw InvalidDimensionException("Coordinates exceed Matrix dimensions.");
                }
            }
            
            inline int coordToIndex(int x, int y) {
                return x + y * dimRows;
            }
            
        public:
            Matrix() {}
            
            Matrix(float elems...) {
                va_list args;
                va_start(args, elems);

                for(int y = 0; y < dimRows; ++y) {
                    for(int x = 0; x < dimCols; ++x) {
                        this->elements[this->coordToIndex(x, y)] = va_arg(args, float);
                    }
                }

                va_end(args);
            }
            
            Matrix(const Matrix& orig) {
                std::memcpy(this->elements, orig.elements, dimCols * dimRows * sizeof(float));
            }
            
        };
    }
}

#endif /* MATRIX_H */

