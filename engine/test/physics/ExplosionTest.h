#ifndef EXPLOSIONTEST_H
#define EXPLOSIONTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "physics/Explosion.h"

#include "../macros.h"

using namespace engine::physics;
using namespace Eigen;

class ExplosionTest : public CppUnit::TestFixture, engine::physics::Explosion {
    CPPUNIT_TEST_SUITE(ExplosionTest);
//    CPPUNIT_TEST(testSimple);
    CPPUNIT_TEST(testPartialSurface);
    CPPUNIT_TEST_SUITE_END();

public:
    ExplosionTest() : Explosion({0, 0, 0}, 1, 1) {}
    
private:
    void testSimple() {
        Explosion::operator=(Explosion({0, 0, 0}, 1, 1));
        
        ObjectProperties props(
                Vector3f(1.5, 0, 0),
                {0},
                Matrix<float, 1, 1>(1),
                Matrix<float, 1, 1>(1)
            );
        
        this->setSecondsSinceStart(1);
        auto forces = this->getForceOnVertices(props);
        CPPUNIT_ASSERT_EQUAL(static_cast<VectorXf::Index>(0), forces.rows());
        
        this->setSecondsSinceStart(2);
        forces = this->getForceOnVertices(props);
        
        VectorXf expected(3);
        expected << 3.05483e-5, 0, 0;
        CPPUNIT_ASSERT_EQUAL(static_cast<VectorXf::Index>(3), forces.rows());
        CPPUNIT_ASSERT_EQUAL(true, forces.isApprox(expected));
        
        this->setSecondsSinceStart(3);
        forces = this->getForceOnVertices(props);
        CPPUNIT_ASSERT_EQUAL(static_cast<VectorXf::Index>(0), forces.rows());
    }
    
    void testPartialSurface() {
        Explosion::operator=(Explosion({0, 0, 0}, 1, 1));
        
        VectorXf vectors(9);
        vectors << 
                1.5, 0, 0,
                2, 0, 0,
                2.5, 0, 0;
        ObjectProperties props(
                vectors,
                {0, 2},
                Matrix<float, 2, 1>(1, 1),
                Matrix<float, 3, 1>(1, 1, 1)
            );
        
        this->setSecondsSinceStart(1);
        auto forces = this->getForceOnVertices(props);
        CPPUNIT_ASSERT_EQUAL(static_cast<VectorXf::Index>(0), forces.rows());
        
        this->setSecondsSinceStart(2.1);
        forces = this->getForceOnVertices(props);
        
        VectorXf expected(9);
        expected << 3.05483e-5, 0, 0,
                0, 0, 0,
                0, 0, 0;
        CPPUNIT_ASSERT_EQUAL(static_cast<VectorXf::Index>(9), forces.rows());
        CPPUNIT_ASSERT_MESSAGE("Only first vector should be affected.", forces.isApprox(expected));
        
        this->setSecondsSinceStart(3);
        forces = this->getForceOnVertices(props);
        
        expected << 0, 0, 0,
                0, 0, 0,
                1.0206e-5, 0, 0;
        CPPUNIT_ASSERT_EQUAL(static_cast<VectorXf::Index>(9), forces.rows());
        CPPUNIT_ASSERT_MESSAGE("Only last vector should be affected.", forces.isApprox(expected));
    }
};

#endif /* EXPLOSIONTEST_H */

