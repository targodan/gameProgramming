#include "MatrixSq.h"

namespace engine {
    namespace math {
        template<>
        float MatrixSq<0>::determinant() const {
            return 1;
        }
        
        template<>
        float MatrixSq<1>::determinant() const {
            return this->elements[0];
        }
        
        template<>
        float MatrixSq<2>::determinant() const {
            return this->elements[this->coordToIndex(0, 0)] * this->elements[this->coordToIndex(1, 1)]
                    - this->elements[this->coordToIndex(1, 0)] * this->elements[this->coordToIndex(0, 1)];
        }
        
        template<>
        float MatrixSq<3>::determinant() const {
            return    this->elements[this->coordToIndex(0, 0)]
                    * this->elements[this->coordToIndex(1, 1)]
                    * this->elements[this->coordToIndex(2, 2)]
                    
                    + this->elements[this->coordToIndex(1, 0)]
                    * this->elements[this->coordToIndex(2, 1)]
                    * this->elements[this->coordToIndex(0, 2)]
                    
                    + this->elements[this->coordToIndex(2, 0)]
                    * this->elements[this->coordToIndex(0, 1)]
                    * this->elements[this->coordToIndex(1, 2)]
                    
                    - this->elements[this->coordToIndex(2, 0)]
                    * this->elements[this->coordToIndex(1, 1)]
                    * this->elements[this->coordToIndex(0, 2)]
                    
                    - this->elements[this->coordToIndex(1, 0)]
                    * this->elements[this->coordToIndex(0, 1)]
                    * this->elements[this->coordToIndex(2, 2)]
                    
                    - this->elements[this->coordToIndex(0, 0)]
                    * this->elements[this->coordToIndex(2, 1)]
                    * this->elements[this->coordToIndex(1, 2)];
        }
        
        template<>
        MatrixSq<1> MatrixSq<1>::invert() const {
            MatrixSq<1> ret;
            ret.elements[0] = 1. / this->elements[0];
            return ret;
        }
        
        template<>
        MatrixSq<2> MatrixSq<2>::invert() const {
            MatrixSq<2> ret;
            
            float rDet = 1. / this->determinant();
            
            ret.elements[ret.coordToIndex(0, 0)] =
                    rDet * this->elements[this->coordToIndex(1, 1)];
            ret.elements[ret.coordToIndex(1, 0)] =
                    -rDet * this->elements[this->coordToIndex(1, 0)];
            ret.elements[ret.coordToIndex(0, 1)] =
                    -rDet * this->elements[this->coordToIndex(0, 1)];
            ret.elements[ret.coordToIndex(1, 1)] =
                    rDet * this->elements[this->coordToIndex(0, 0)];
            
            return ret;
        }
        
        template<>
        MatrixSq<3> MatrixSq<3>::invert() const {
            MatrixSq<3> ret;
            
            float rDet = 1. / this->determinant();
            
            ret.elements[ret.coordToIndex(0, 0)] = rDet * (
                      this->elements[this->coordToIndex(1, 1)]
                    * this->elements[this->coordToIndex(2, 2)]
                    - this->elements[this->coordToIndex(2, 1)]
                    * this->elements[this->coordToIndex(1, 2)]);
            
            ret.elements[ret.coordToIndex(1, 0)] = rDet * (
                      this->elements[this->coordToIndex(2, 0)]
                    * this->elements[this->coordToIndex(1, 2)]
                    - this->elements[this->coordToIndex(1, 0)]
                    * this->elements[this->coordToIndex(2, 2)]);
            
            ret.elements[ret.coordToIndex(2, 0)] = rDet * (
                      this->elements[this->coordToIndex(1, 0)]
                    * this->elements[this->coordToIndex(2, 1)]
                    - this->elements[this->coordToIndex(2, 0)]
                    * this->elements[this->coordToIndex(1, 1)]);
            
            ret.elements[ret.coordToIndex(0, 1)] = rDet * (
                      this->elements[this->coordToIndex(2, 1)]
                    * this->elements[this->coordToIndex(0, 2)]
                    - this->elements[this->coordToIndex(0, 1)]
                    * this->elements[this->coordToIndex(2, 2)]);
            
            ret.elements[ret.coordToIndex(1, 1)] = rDet * (
                      this->elements[this->coordToIndex(0, 0)]
                    * this->elements[this->coordToIndex(2, 2)]
                    - this->elements[this->coordToIndex(2, 0)]
                    * this->elements[this->coordToIndex(0, 2)]);
            
            ret.elements[ret.coordToIndex(2, 1)] = rDet * (
                      this->elements[this->coordToIndex(2, 0)]
                    * this->elements[this->coordToIndex(0, 1)]
                    - this->elements[this->coordToIndex(0, 0)]
                    * this->elements[this->coordToIndex(2, 1)]);
            
            ret.elements[ret.coordToIndex(0, 2)] = rDet * (
                      this->elements[this->coordToIndex(0, 1)]
                    * this->elements[this->coordToIndex(1, 2)]
                    - this->elements[this->coordToIndex(1, 1)]
                    * this->elements[this->coordToIndex(0, 2)]);
            
            ret.elements[ret.coordToIndex(1, 2)] = rDet * (
                      this->elements[this->coordToIndex(1, 0)]
                    * this->elements[this->coordToIndex(0, 2)]
                    - this->elements[this->coordToIndex(0, 0)]
                    * this->elements[this->coordToIndex(1, 2)]);
            
            ret.elements[ret.coordToIndex(2, 2)] = rDet * (
                      this->elements[this->coordToIndex(0, 0)]
                    * this->elements[this->coordToIndex(1, 1)]
                    - this->elements[this->coordToIndex(1, 0)]
                    * this->elements[this->coordToIndex(0, 1)]);
            
            return ret;
        }
    }
}