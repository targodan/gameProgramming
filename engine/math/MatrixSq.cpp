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
    }
}