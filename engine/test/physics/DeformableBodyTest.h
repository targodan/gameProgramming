#ifndef DEFORMABLEBODYTEST_H
#define DEFORMABLEBODYTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "physics/DeformableBody.h"

#include "../macros.h"

class DeformableBodyTest : public CppUnit::TestFixture, engine::physics::DeformableBody {
    CPPUNIT_TEST_SUITE(DeformableBodyTest);
    CPPUNIT_TEST(test);
    CPPUNIT_TEST_SUITE_END();

public:
    DeformableBodyTest() : TestFixture(), DeformableBody(
            engine::renderer::Mesh({engine::renderer::Vertex({0, 0, 0.5}), engine::renderer::Vertex({0.5, 0, -0.5}), engine::renderer::Vertex({-0.5, 0, -0.5}), engine::renderer::Vertex({0, 0.5, 0})}, {0, 1, 3, 0, 3, 2, 1, 2, 3}),
            10, 0.1, 0.05e9, 0.27, 1/60.) {}
    
private:
    void test() {
        Eigen::Matrix<float, 12, 1> force = Eigen::Matrix<float, 12, 1>::Zero();
        force[2] = -100;
        this->step(1/60., force);
        std::cout << std::endl << this->lastVelocities;
    }
};

#endif /* DEFORMABLEBODYTEST_H */

