#ifndef MATRIXTEST_H
#define MATRIXTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "../macros.h"

#include "math/Matrix.h"
#include "math/MatrixSq.h"

using namespace engine::math;

class MatrixTest : public CppUnit::TestFixture {
public:
    CPPUNIT_TEST_SUITE(MatrixTest);
    CPPUNIT_TEST(testToString);
    CPPUNIT_TEST(testMatrixAddition);
    CPPUNIT_TEST(testMatrixMultiplication);
    CPPUNIT_TEST(testDeterminant);
    CPPUNIT_TEST(testInverse);
    CPPUNIT_TEST_SUITE_END();

    void testToString() {
        Matrix<4, 3> m = {
            1, 2, 3, 4,
            5, 6.66666666666, 7, 8,
            9, 10, 11, 12
        };
        std::cout << std::endl << m << std::endl;
        std::cout << std::endl << MatrixSq<3>::identity() << std::endl;
    }
    
    void testMatrixAddition() {
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
        
        Matrix<3, 2> m3 = m2.transpose();
        
        Matrix<2, 3> mRes = {
            12,    8,
            -4.4, 1.8,
            0.1,  0
        };
        
        CPPUNIT_ASSERT_EQUAL(mRes, m1 + m2);
        
        Matrix<3, 3> mRes2 = {
            32,    -35.2,   -63.6,
            -16,   -11.2,   -40.2,
            0,     -25.6,   -64
        };
        
        CPPUNIT_ASSERT_EQUAL(mRes2, m1 * m3);
    }
    
    void testMatrixMultiplication() {
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
        
        Matrix<3, 2> m3 = m2.transpose();
        
        Matrix<3, 3> mRes = {
            32,    -35.2,   -63.6,
            -16,   -11.2,   -40.2,
            0,     -25.6,   -64
        };
        
        CPPUNIT_ASSERT_EQUAL(mRes, m1 * m3);
    }
    
    void testDeterminant() {
        MatrixSq<2> m1 = {
             4,  8,
            -2,  5
        };
        
        CPPUNIT_ASSERT_EQUAL(36.f, m1.determinant());
        
        MatrixSq<3> m2 = {
             4,  8,  5,
            -2,  5,  2, 
             0,  8,  5
        };
        
        CPPUNIT_ASSERT_EQUAL(36.f, m2.determinant());
    }
    
    void testInverse() {
        MatrixSq<2> m1 = {
             4,  8,
            -2,  5
        };
        MatrixSq<2> m1Res = {
             0.1388888888,  -0.2222222222,
             0.0555555555,   0.1111111111
        };
        
        CPPUNIT_ASSERT_EQUAL(m1Res, m1.inverse());
        
        MatrixSq<3> m2 = {
             4,  8,  5,
            -2,  5,  2, 
             0,  8,  5
        };
        MatrixSq<3> m2Res = {
             0.25,          0,             -0.25,
             0.2777777777,  0.5555555555,  -0.5, 
            -0.4444444444, -0.8888888888,   1
        };
        
        CPPUNIT_ASSERT_EQUAL(m2Res, m2.inverse());
    }
};

#endif /* MATRIXTEST_H */

