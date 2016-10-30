#ifndef MATRIXSQ_H
#define MATRIXSQ_H

#include <cstdarg>

#include "Matrix.h"

namespace engine {
    namespace math {
        template<int dimension>
        class MatrixSq : public Matrix<dimension, dimension> {
        public:
            MatrixSq() {}
            
            MatrixSq(float elems...) {
                va_list args;
                va_start(args, elems);

                for(int y = 0; y < dimension; ++y) {
                    for(int x = 0; x < dimension; ++x) {
                        this->elements[this->coordToIndex(x, y)] = static_cast<float>(va_arg(args, double));
                    }
                }

                va_end(args);
            }
            
            float determinant() const {
                // Using Laplace recursively.
                // Recursion ends on specialization for dimension == 3.
                int sign = 1;
                float det = 0;
                for(int y = 0; y < dimension; ++y) {
                    for(int x = 0; x < dimension; ++x) {
                        if(!IS_FLOAT_ZERO(this->elements[this->coordToIndex(x, y)])) {
                            det += sign * this->elements[this->coordToIndex(x, y)]
                                    * this->getSubMatrixWithoutAxis(x, y).determinant();
                        }
                        
                        sign = sign == 1 ? -1 : 1;
                    }
                }
                
                return det;
            }
        };
    }
}

#endif /* MATRIXSQ_H */

