#ifndef SYSTEMMANAGERTEST_H
#define SYSTEMMANAGERTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <memory>

#include "../macros.h"

#include "ECS/SystemManager.h"

#include "SystemImplementations.h"

using namespace engine::ECS;
using std::shared_ptr;

class SystemManagerTest : public CPPUNIT_NS::TestFixture, public SystemManager {
    CPPUNIT_TEST_SUITE(SystemManagerTest);
    CPPUNIT_TEST(testBuildDependencyGraph);
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
};

#endif /* SYSTEMMANAGERTEST_H */

