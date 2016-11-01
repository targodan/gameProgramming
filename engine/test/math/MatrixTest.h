#ifndef MATRIXTEST_H
#define MATRIXTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include "../out.h"

#include "../macros.h"

#include "math/Matrix.h"

using namespace engine::math;

class MatrixTest : public CppUnit::TestFixture {
public:
    CPPUNIT_TEST_SUITE(MatrixTest);
    CPPUNIT_TEST(testToString);
    CPPUNIT_TEST(testAddition);
//    CPPUNIT_TEST(test);
    CPPUNIT_TEST_SUITE_END();

    void testToString() {
        Matrix<4, 3> m = {
            1, 2, 3, 4,
            5, 6.66666666666, 7, 8,
            9, 10, 11, 12
        };
        out << std::endl << m << std::endl;
    }
    
    void testAddition() {
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
            0.1,  0
        };
        
        CPPUNIT_ASSERT_EQUAL(mRes, m1 + m2);
    }
};

#endif /* MATRIXTEST_H */

