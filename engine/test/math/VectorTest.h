#ifndef VECTORTEST_H
#define VECTORTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <iostream>

#include "../macros.h"

#include "math/Vector.h"

using namespace engine::math;

class VectorTest : public CppUnit::TestFixture {
public:
    CPPUNIT_TEST_SUITE(VectorTest);
    CPPUNIT_TEST(testConstructor);
    CPPUNIT_TEST_SUITE_END();
    
    void testConstructor() {
        Vector<2> v0;
        CPPUNIT_ASSERT(v0[0] == 0 && v0[1] == 0);
        
        Vector<2> v = {1, 4.7};
        CPPUNIT_ASSERT(FLOAT_EQUALS(v[0], 1) && FLOAT_EQUALS(v[1], 4.7));
        
        Vector<2> vCpy = v;
        v[0] = 666;
        CPPUNIT_ASSERT(FLOAT_EQUALS(vCpy[0], 1) && FLOAT_EQUALS(vCpy[1], 4.7));
    }
};

#endif /* VECTORTEST_H */

