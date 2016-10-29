#include <iostream>

#include "math/Vector.h"

using namespace engine::math;

void test() {
    Vector<3> v(0., 1., 0.);
    float test = v[0];
    v[1] = test;
    
    v += v + v + v;
}
