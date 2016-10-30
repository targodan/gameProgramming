#include <iostream>

#include "math/Vector3.h"

using namespace engine::math;

void test() {
    Vector<3> v(0., 1., 0.);
    float test = v[0];
    v[1] = test;
    
    Vector3 v3(0., 0., 5.2);
    v3 = v3.add(v);
    
    v += v + v * 2 + 2 *v;
}
