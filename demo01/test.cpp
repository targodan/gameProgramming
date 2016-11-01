#include <iostream>

#include "math/Matrix.h"

using namespace engine::math;

int main(int argc, char** argv) {
    Matrix<2, 3> m1 = {
        4, 8,
        -2, 5,
        0, 8
    };

    Matrix<2, 3> m2 = {
        8,    0,
        -2.4, -3.2,
        0.1,  -8
    };

    Matrix<2, 3> mRes = {
        12,    8,
        -4.4, 1.8,
        -0.1,  0
    };

    std::cout << std::endl << "pre" << std::endl;
    m1.add(m2);
    std::cout << std::endl << "post" << std::endl;
       m1 = m1 + m2;
//        CPPUNIT_ASSERT_EQUAL(mRes, m1 + m2);

    return 0;
}
