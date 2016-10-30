#ifndef MATRIXSQ_H
#define MATRIXSQ_H

#include <cstdarg>

#include "Matrix.h"
#include "../WTFException.h"

namespace engine {
    namespace math {
        template<unsigned int dimension>
        class MatrixSq : public Matrix<dimension, dimension> {
        public:
            MatrixSq() : Matrix<dimension, dimension>() {}
            
            MatrixSq(const Matrix<dimension, dimension>& orig) {
                std::memcpy(this->elements, orig.elements, dimension * dimension * sizeof(float));
            }
            
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
            
            MatrixSq<dimension>& operator=(const Matrix<dimension, dimension>& orig) {
                std::memcpy(this->elements, orig.elements, dimension * dimension * sizeof(float));
                return *this;
            }
            
            static MatrixSq<dimension> NewIdentity() {
                MatrixSq<dimension> ret;
                std::fill(ret.elements, ret.elements + dimension, 0.);
                return ret;
            } 
            
            float determinant() const {
                // Using Laplace recursively.
                // Recursion ends on specialization for dimension == 3.
                int sign = 1;
                float det = 0;
                for(unsigned int y = 0; y < dimension; ++y) {
                    for(unsigned int x = 0; x < dimension; ++x) {
                        if(!IS_FLOAT_ZERO(this->elements[this->coordToIndex(x, y)])) {
                            MatrixSq<dimension-1> m(this->getSubMatrixWithoutAxis(x, y));
                            det += sign * this->elements[this->coordToIndex(x, y)]
                                    * m.determinant();
                        }
                        
                        sign = sign == 1 ? -1 : 1;
                    }
                }
                
                return det;
            }
            
            MatrixSq<dimension> invert() const {
                // Only implemented for dimensions 3, 2 and 1.
                // This is done via template specialization.
                throw WTFException("Matrix inversion is only supported for dimensions 3, 2 and 1.");
            }
        };
        
        template<>
        float MatrixSq<1>::determinant() const;
        
        template<>
        float MatrixSq<2>::determinant() const;
        
        template<>
        float MatrixSq<3>::determinant() const;
        
        template<>
        MatrixSq<1> MatrixSq<1>::invert() const;
        
        template<>
        MatrixSq<2> MatrixSq<2>::invert() const;
        
        template<>
        MatrixSq<3> MatrixSq<3>::invert() const;
    }
}

#endif /* MATRIXSQ_H */

