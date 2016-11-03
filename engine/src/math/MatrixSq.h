#ifndef MATRIXSQ_H
#define MATRIXSQ_H

#include <initializer_list>

#include "Matrix.h"
#include "../WTFException.h"

namespace engine {
    namespace math {
        template<unsigned int dimension>
        class MatrixSq : public Matrix<dimension, dimension> {
        public:
            MatrixSq() : Matrix<dimension, dimension>() {}
            
            MatrixSq(std::initializer_list<float> list) : MatrixSq() {
                if(list.size() < dimension * dimension) {
                    throw InvalidDimensionException("Not enough elements in list.");
                } else if(list.size() > dimension * dimension) {
                    throw InvalidDimensionException("Too many elements in list.");
                }
                std::copy(list.begin(), list.end(), this->elements.begin());
            }
            
            MatrixSq(const Matrix<dimension, dimension>& orig) {
                std::copy(orig.elements.begin(), orig.elements.end(), this->elements.begin());
            }
            
            MatrixSq<dimension>& operator=(const Matrix<dimension, dimension>& orig) {
                std::copy(orig.elements.begin(), orig.elements.end(), this->elements.begin());
                return *this;
            }
            
            MatrixSq<dimension>& operator=(const MatrixSq<dimension>& orig) {
                std::copy(orig.elements.begin(), orig.elements.end(), this->elements.begin());
                return *this;
            }
            
            static MatrixSq<dimension> identity() {
                MatrixSq<dimension> ret;
                std::fill(ret.elements.begin(), ret.elements.end(), 0.);
                for(unsigned int i = 0; i < dimension; ++i) {
                    ret.elements[ret.coordToIndex(i, i)] = 1;
                }
                return ret;
            }
            
            inline unsigned int getDimension() const {
                return dimension;
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
            
            MatrixSq<dimension> inverse() const {
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
        MatrixSq<1> MatrixSq<1>::inverse() const;
        
        template<>
        MatrixSq<2> MatrixSq<2>::inverse() const;
        
        template<>
        MatrixSq<3> MatrixSq<3>::inverse() const;
    }
}

#endif /* MATRIXSQ_H */

