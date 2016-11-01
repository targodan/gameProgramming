#ifndef VECTORTEST_H
#define VECTORTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include "../out.h"

#include "../macros.h"

#include "math/Vector.h"
#include "math/Vector3.h"

using namespace engine::math;

class VectorTest : public CppUnit::TestFixture {
public:
    CPPUNIT_TEST_SUITE(VectorTest);
    CPPUNIT_TEST(testConstructor);
    CPPUNIT_TEST(testAddition);
    CPPUNIT_TEST(testSubtraction);
    CPPUNIT_TEST(testMultiplication);
    CPPUNIT_TEST(testCross);
    CPPUNIT_TEST(testToString);
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
    
    void testAddition() {
        Vector<3> v1 = {-2, 4.5, 2};
        Vector<3> v2 = {2, 1.5, -3};
        
        Vector<3> vRes = {0, 6, -1};
        CPPUNIT_ASSERT_EQUAL(vRes, v1 + v2);
        
        v1.add(v2);
        CPPUNIT_ASSERT_EQUAL(vRes, v1);
    }
    
    void testSubtraction() {
        Vector<3> v1 = {-2, 4.5, 2};
        Vector<3> v2 = {2, 1.5, -3};
        
        Vector<3> vRes = {-4, 3, 5};
        CPPUNIT_ASSERT_EQUAL(vRes, v1 - v2);
        
        v1.sub(v2);
        CPPUNIT_ASSERT_EQUAL(vRes, v1);
    }
    
    void testMultiplication() {
        Vector<3> v1 = {-2, 4.5, 2};
        Vector<3> vRes = {-3, 6.75, 3};
        CPPUNIT_ASSERT_EQUAL(v1 * 1.5, vRes);
        
        v1.mul(1.5);
        CPPUNIT_ASSERT_EQUAL(v1, vRes);
    }
    
    void testCross() {
        Vector3 v1 = {-2, 4.5, 2};
        Vector3 v2 = {2, 1.5, -3};
        
        Vector3 vRes = {-16.5, -2, -12};
        CPPUNIT_ASSERT_EQUAL(vRes, v1 % v2);
        
        v1 %= v2;
        CPPUNIT_ASSERT_EQUAL(vRes, v1);
    }
    
    void testToString() {
        Vector<3> v = {42.1, 65.9, -24};
        out << std::endl << v << std::endl;
    }
};

#endif /* VECTORTEST_H */

