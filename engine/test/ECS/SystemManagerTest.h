#ifndef SYSTEMMANAGERTEST_H
#define SYSTEMMANAGERTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <memory>

#include "../macros.h"

#include "util/ostream_helper.h"
#include "ECS/DependencyException.h"
#include "ECS/SystemManager.h"

#include "SystemImplementations.h"

using namespace engine::ECS;
using std::shared_ptr;

class SystemManagerTest : public CPPUNIT_NS::TestFixture, public SystemManager {
    CPPUNIT_TEST_SUITE(SystemManagerTest);
    CPPUNIT_TEST(testBuildDependencyGraph);
    CPPUNIT_TEST(testIsGraphCircular_true);
    CPPUNIT_TEST(testIsGraphCircular_false);
    CPPUNIT_TEST(testMergeDependencySublists);
    CPPUNIT_TEST_SUITE_END();

public:
    void tearDown() override {
        this->systemsPreAnalysis.clear();
        this->systems.clear();
    }
    
private:
    void testBuildDependencyGraph() {
        this->enableSystem<TestSystem1>();
        this->enableSystem<TestSystem2>();
        this->enableSystem<TestSystem3>();
        this->enableSystem<TestSystem4>();
        this->enableSystem<TestSystem5>();
        this->enableSystem<TestSystem6>();
        this->enableSystem<TestSystem7>();
        this->enableSystem<TestSystem8>();
        
        auto roots = this->buildDependencyGraph();
        
        CPPUNIT_ASSERT_EQUAL(3ul, roots.size());
        
        CPPUNIT_ASSERT_EQUAL(std::string("TestSystem1"), roots[0]->system->getSystemName());
        CPPUNIT_ASSERT_EQUAL(1ul, roots[0]->children.size());
        CPPUNIT_ASSERT_EQUAL(std::string("TestSystem3"), roots[0]->children[0]->system->getSystemName());
        
        CPPUNIT_ASSERT_EQUAL(std::string("TestSystem2"), roots[1]->system->getSystemName());
        CPPUNIT_ASSERT_EQUAL(2ul, roots[1]->children.size());
        CPPUNIT_ASSERT_EQUAL(std::string("TestSystem3"), roots[1]->children[0]->system->getSystemName());
        CPPUNIT_ASSERT_EQUAL(std::string("TestSystem4"), roots[1]->children[1]->system->getSystemName());
        
        CPPUNIT_ASSERT_EQUAL(1ul, roots[1]->children[0]->children.size());
        CPPUNIT_ASSERT_EQUAL(std::string("TestSystem5"), roots[1]->children[0]->children[0]->system->getSystemName());
        CPPUNIT_ASSERT_EQUAL(1ul, roots[1]->children[1]->children.size());
        CPPUNIT_ASSERT_EQUAL(std::string("TestSystem5"), roots[1]->children[1]->children[0]->system->getSystemName());
        
        CPPUNIT_ASSERT_EQUAL(2ul, roots[1]->children[0]->children[0]->children.size());
        CPPUNIT_ASSERT_EQUAL(std::string("TestSystem7"), roots[1]->children[0]->children[0]->children[0]->system->getSystemName());
        CPPUNIT_ASSERT_EQUAL(std::string("TestSystem8"), roots[1]->children[0]->children[0]->children[1]->system->getSystemName());
        
        CPPUNIT_ASSERT_EQUAL(std::string("TestSystem6"), roots[2]->system->getSystemName());
        CPPUNIT_ASSERT_EQUAL(1ul, roots[2]->children.size());
        CPPUNIT_ASSERT_EQUAL(std::string("TestSystem8"), roots[2]->children[0]->system->getSystemName());
        
        CPPUNIT_ASSERT_EQUAL(0ul, roots[1]->children[0]->children[0]->children[0]->children.size());
        CPPUNIT_ASSERT_EQUAL(0ul, roots[1]->children[0]->children[0]->children[1]->children.size());
    }
    
    void testIsGraphCircular_true() {
        this->enableSystem<LoopTest1>();
        this->enableSystem<LoopTest2>();
        this->enableSystem<LoopTest3>();
        
        auto roots = this->buildDependencyGraph();
        CPPUNIT_ASSERT_EQUAL(true, this->isGraphCircular(roots));
        
        this->enableSystem<LoopTest0>();
        
        roots = this->buildDependencyGraph();
        CPPUNIT_ASSERT_EQUAL(true, this->isGraphCircular(roots));
    }
    
    void testIsGraphCircular_false() {
        this->enableSystem<TestSystem1>();
        this->enableSystem<TestSystem2>();
        this->enableSystem<TestSystem3>();
        this->enableSystem<TestSystem4>();
        this->enableSystem<TestSystem5>();
        this->enableSystem<TestSystem6>();
        this->enableSystem<TestSystem7>();
        this->enableSystem<TestSystem8>();
        
        auto roots = this->buildDependencyGraph();
        CPPUNIT_ASSERT_EQUAL(false, this->isGraphCircular(roots));
    }
    
    void testMergeDependencySublists() {
        auto s1 = std::make_shared<TestSystem1>();
        auto s2 = std::make_shared<TestSystem2>();
        auto s3 = std::make_shared<TestSystem3>();
        auto s4 = std::make_shared<TestSystem4>();
        auto s5 = std::make_shared<TestSystem5>();
        auto s6 = std::make_shared<TestSystem6>();
        auto s7 = std::make_shared<TestSystem7>();
        auto s8 = std::make_shared<TestSystem8>();
        
        {
            vector<shared_ptr<System>> prim = {s1, s2, s3, s4, s5};
            vector<shared_ptr<System>> sec = {s2, s6, s7, s8, s5};
            vector<shared_ptr<System>> exp = {s1, s2, s6, s7, s8, s3, s4, s5};
            auto result = this->mergeDependencySublists(prim, sec);
            CPPUNIT_ASSERT_EQUAL(exp, result);
        }
        
        {
            vector<shared_ptr<System>> prim = {s1, s2, s3, s4, s5};
            vector<shared_ptr<System>> sec = {s2, s6, s5};
            vector<shared_ptr<System>> exp = {s1, s2, s6, s3, s4, s5};
            auto result = this->mergeDependencySublists(prim, sec);
            CPPUNIT_ASSERT_EQUAL(exp, result);
        }
        
        {
            vector<shared_ptr<System>> prim = {s1, s2, s3, s4, s5};
            vector<shared_ptr<System>> sec = {s2, s6};
            CPPUNIT_ASSERT_THROW_MESSAGE("No merge point => should throw.",
                    this->mergeDependencySublists(prim, sec),
                    engine::WTFException);
        }
        
        {
            vector<shared_ptr<System>> prim = {s1, s2, s3, s4, s5};
            vector<shared_ptr<System>> sec = {s6, s7, s3};
            CPPUNIT_ASSERT_THROW_MESSAGE("No split point => should throw.",
                    this->mergeDependencySublists(prim, sec),
                    engine::WTFException);
        }
    }
};

#endif /* SYSTEMMANAGERTEST_H */

