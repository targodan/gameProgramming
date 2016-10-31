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
    CPPUNIT_TEST_SUITE_END();

    void testToString() {
        Matrix<4, 3> m = {
            1, 2, 3, 4,
            5, 6.66666666666, 7, 8,
            9, 10, 11, 12
        };
        out << std::endl << m << std::endl;
    }
};

#endif /* MATRIXTEST_H */

