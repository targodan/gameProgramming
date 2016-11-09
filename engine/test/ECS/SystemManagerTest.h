#ifndef SYSTEMMANAGERTEST_H
#define SYSTEMMANAGERTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <memory>
#include <random>
#include <algorithm>

#include "../macros.h"

#include "util/ostream_helper.h"
#include "ECS/DependencyException.h"
#include "ECS/SystemManager.h"

#include "SystemImplementations.h"

using namespace engine::ECS;
using std::shared_ptr;

template<class T>
bool vectorContainsABeforeB(std::vector<T> v, const T& A, const T& B) {
    auto itA = std::find(v.begin(), v.end(), A);
    auto itB = std::find(v.begin(), v.end(), B);
    if(itA == v.end() || itB == v.end()) {
        return false;
    }
    return itA < itB;
}

template<class T>
typename std::vector<T>::const_iterator isSet(const std::vector<T>& v) {
    for(auto it = v.begin(); it != v.end(); ++it) {
        auto fit = std::find(it+1, v.end(), *it);
        if(fit != v.end()) {
            return fit;
        }
    }
    return v.end();
}

EntityManager em;

class SystemManagerTest : public CPPUNIT_NS::TestFixture, public SystemManager {
    CPPUNIT_TEST_SUITE(SystemManagerTest);
    CPPUNIT_TEST(testBuildDependencyGraph);
    CPPUNIT_TEST(testIsGraphCircular_true);
    CPPUNIT_TEST(testIsGraphCircular_false);
    CPPUNIT_TEST_SUITE_END();

public:
    SystemManagerTest() : SystemManager(em) {}
    
    void tearDown() override {
        this->enabledSystems.clear();
        this->dependencyTree.clear();
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
        CPPUNIT_ASSERT_EQUAL(std::string("TestSystem7"), roots[1]->children[1]->children[0]->children[0]->system->getSystemName());
        CPPUNIT_ASSERT_EQUAL(std::string("TestSystem8"), roots[1]->children[1]->children[0]->children[1]->system->getSystemName());
        
        CPPUNIT_ASSERT_EQUAL(std::string("TestSystem6"), roots[2]->system->getSystemName());
        CPPUNIT_ASSERT_EQUAL(1ul, roots[2]->children.size());
        CPPUNIT_ASSERT_EQUAL(std::string("TestSystem8"), roots[2]->children[0]->system->getSystemName());
        
        CPPUNIT_ASSERT_EQUAL(0ul, roots[1]->children[1]->children[0]->children[0]->children.size());
        CPPUNIT_ASSERT_EQUAL(0ul, roots[1]->children[1]->children[0]->children[1]->children.size());
        
        auto s7 = roots[1]->children[1]->children[0]->children[0];
        auto s8 = roots[1]->children[1]->children[0]->children[1];
        
        CPPUNIT_ASSERT_EQUAL(0ul, roots[0]->parents.size());
        CPPUNIT_ASSERT_EQUAL(0ul, roots[1]->parents.size());
        CPPUNIT_ASSERT_EQUAL(0ul, roots[2]->parents.size());
        
        CPPUNIT_ASSERT_EQUAL(std::string("TestSystem5"), s8->parents[1].lock()->system->getSystemName());
        CPPUNIT_ASSERT_EQUAL(std::string("TestSystem6"), s8->parents[0].lock()->system->getSystemName());
        CPPUNIT_ASSERT_EQUAL(2ul, s8->parents.size());
        
        CPPUNIT_ASSERT_EQUAL(std::string("TestSystem3"), s8->parents[1].lock()->parents[0].lock()->system->getSystemName());
        CPPUNIT_ASSERT_EQUAL(std::string("TestSystem4"), s8->parents[1].lock()->parents[1].lock()->system->getSystemName());
        CPPUNIT_ASSERT_EQUAL(2ul, s8->parents[1].lock()->parents.size());
        
        CPPUNIT_ASSERT_EQUAL(std::string("TestSystem1"), s8->parents[1].lock()->parents[0].lock()->parents[0].lock()->system->getSystemName());
        CPPUNIT_ASSERT_EQUAL(std::string("TestSystem2"), s8->parents[1].lock()->parents[0].lock()->parents[1].lock()->system->getSystemName());
        CPPUNIT_ASSERT_EQUAL(2ul, s8->parents[1].lock()->parents[0].lock()->parents.size());
        
        CPPUNIT_ASSERT_EQUAL(std::string("TestSystem2"), s8->parents[1].lock()->parents[1].lock()->parents[0].lock()->system->getSystemName());
        CPPUNIT_ASSERT_EQUAL(1ul, s8->parents[1].lock()->parents[1].lock()->parents.size());
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
};

#endif /* SYSTEMMANAGERTEST_H */

