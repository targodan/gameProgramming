#include <iostream>

#include "math/Vector3.h"
#include "math/MatrixSq.h"

using namespace engine::math;

void test() {
    Vector<3> v = {0., 1., 0.};
    float test = v[0];
    v[1] = test;
    
    Vector3 v3(0., 0., 5.2);
    v3 = v3.add(v);
    
    v = v3 + 2 * v;
    
    Vector<3> vInit = {1, 2, 3};
    Vector3 vInit3 = {1, 2, 3};
    
    Matrix<3, 3> m = {1, 2, 3,
                      4, 5, 6,
                      7, 8, 9};
    MatrixSq<3> m2 = {1, 2, 3,
                      4, 5, 6,
                      7, 8, 9};
    
    m2[0][1] = m[1][1];
    
    m2.determinant();
    
    m2 = m2.mul(m);
}
