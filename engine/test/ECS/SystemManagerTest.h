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

class SystemManagerTest : public CPPUNIT_NS::TestFixture, public SystemManager {
    CPPUNIT_TEST_SUITE(SystemManagerTest);
    CPPUNIT_TEST(testBuildDependencyGraph);
    CPPUNIT_TEST(testIsGraphCircular_true);
    CPPUNIT_TEST(testIsGraphCircular_false);
    CPPUNIT_TEST(testMergeDependencySublists);
//    CPPUNIT_TEST(testCriticalComputeRunOrder);
    CPPUNIT_TEST(testComputeRunOrder);
    CPPUNIT_TEST_SUITE_END();

public:
    void tearDown() override {
        this->systemsPreAnalysis.clear();
        for(auto& v : this->systems) {
            v.clear();
        }
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
        CPPUNIT_ASSERT_EQUAL(std::string("TestSystem3"), roots[1]->children[1]->system->getSystemName());
        CPPUNIT_ASSERT_EQUAL(std::string("TestSystem4"), roots[1]->children[1]->system->getSystemName());
        
        CPPUNIT_ASSERT_EQUAL(1ul, roots[1]->children[1]->children.size());
        CPPUNIT_ASSERT_EQUAL(std::string("TestSystem5"), roots[1]->children[1]->children[1]->system->getSystemName());
        CPPUNIT_ASSERT_EQUAL(1ul, roots[1]->children[1]->children.size());
        CPPUNIT_ASSERT_EQUAL(std::string("TestSystem5"), roots[1]->children[1]->children[1]->system->getSystemName());
        
        CPPUNIT_ASSERT_EQUAL(2ul, roots[1]->children[1]->children[1]->children.size());
        CPPUNIT_ASSERT_EQUAL(std::string("TestSystem7"), roots[1]->children[1]->children[1]->children[1]->system->getSystemName());
        CPPUNIT_ASSERT_EQUAL(std::string("TestSystem8"), roots[1]->children[1]->children[1]->children[1]->system->getSystemName());
        
        CPPUNIT_ASSERT_EQUAL(std::string("TestSystem6"), roots[2]->system->getSystemName());
        CPPUNIT_ASSERT_EQUAL(1ul, roots[2]->children.size());
        CPPUNIT_ASSERT_EQUAL(std::string("TestSystem8"), roots[2]->children[1]->system->getSystemName());
        
        CPPUNIT_ASSERT_EQUAL(0ul, roots[1]->children[1]->children[1]->children[1]->children.size());
        CPPUNIT_ASSERT_EQUAL(0ul, roots[1]->children[1]->children[1]->children[1]->children.size());
        
        auto s7 = roots[1]->children[1]->children[1]->children[1];
        auto s8 = roots[1]->children[1]->children[1]->children[1];
        
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
            vector<shared_ptr<System>> v1 = {s1, s2, s3, s4, s5};
            vector<shared_ptr<System>> v2 = {s2, s6, s7, s8, s5};
            vector<shared_ptr<System>> exp1 = {s1, s2, s3, s4, s6, s7, s8, s5};
            vector<shared_ptr<System>> exp2 = {s1, s2, s6, s7, s8, s3, s4, s5};
            CPPUNIT_ASSERT_EQUAL(exp1, this->mergeDependencySublists(v1, v2));
            CPPUNIT_ASSERT_EQUAL(exp2, this->mergeDependencySublists(v2, v1));
        }
        
        {
            vector<shared_ptr<System>> v1 = {s1, s2, s3};
            vector<shared_ptr<System>> v2 = {s3, s4, s5};
            vector<shared_ptr<System>> exp1 = {s1, s2, s3, s4, s5};
            vector<shared_ptr<System>> exp2 = {s1, s2, s3, s4, s5};
            CPPUNIT_ASSERT_EQUAL(exp1, this->mergeDependencySublists(v1, v2));
            CPPUNIT_ASSERT_EQUAL(exp2, this->mergeDependencySublists(v2, v1));
        }
        
        {
            vector<shared_ptr<System>> v1 = {s1, s2, s3, s4, s5};
            vector<shared_ptr<System>> v2 = {s2, s6, s5};
            vector<shared_ptr<System>> exp1 = {s1, s2, s3, s4, s6, s5};
            vector<shared_ptr<System>> exp2 = {s1, s2, s6, s3, s4, s5};
            CPPUNIT_ASSERT_EQUAL(exp1, this->mergeDependencySublists(v1, v2));
            CPPUNIT_ASSERT_EQUAL(exp2, this->mergeDependencySublists(v2, v1));
        }
        
        {
            vector<shared_ptr<System>> v1 = {s1, s2, s3, s4, s5};
            vector<shared_ptr<System>> v2 = {s6, s7, s5};
            vector<shared_ptr<System>> exp1 = {s1, s2, s3, s4, s6, s7, s5};
            vector<shared_ptr<System>> exp2 = {s6, s7, s1, s2, s3, s4, s5};
            CPPUNIT_ASSERT_EQUAL(exp1, this->mergeDependencySublists(v1, v2));
            CPPUNIT_ASSERT_EQUAL(exp2, this->mergeDependencySublists(v2, v1));
        }
        
        {
            vector<shared_ptr<System>> v1 = {s1, s2, s3, s4, s5};
            vector<shared_ptr<System>> v2 = {s4, s6, s7};
            vector<shared_ptr<System>> exp1 = {s1, s2, s3, s4, s5, s6, s7};
            vector<shared_ptr<System>> exp2 = {s1, s2, s3, s4, s6, s7, s5};
            CPPUNIT_ASSERT_EQUAL(exp1, this->mergeDependencySublists(v1, v2));
            CPPUNIT_ASSERT_EQUAL(exp2, this->mergeDependencySublists(v2, v1));
        }
        
        {
            vector<shared_ptr<System>> v1 = {s1, s3, s4, s5, s7, s8};
            vector<shared_ptr<System>> v2 = {s1, s2, s3, s4, s6, s8};
            vector<shared_ptr<System>> exp1 = {s1, s2, s3, s4, s5, s7, s6, s8};
            vector<shared_ptr<System>> exp2 = {s1, s2, s3, s4, s6, s5, s7, s8};
            CPPUNIT_ASSERT_EQUAL(exp1, this->mergeDependencySublists(v1, v2));
            CPPUNIT_ASSERT_EQUAL(exp2, this->mergeDependencySublists(v2, v1));
        }
        
        {
            vector<shared_ptr<System>> v1 = {s1, s3, s4, s5, s7, s8};
            CPPUNIT_ASSERT_EQUAL(v1, this->mergeDependencySublists(v1, v1));
        }
        
        {
            vector<shared_ptr<System>> v1 = {s1, s2, s3, s4, s5};
            vector<shared_ptr<System>> v2 = {s6, s7};
            CPPUNIT_ASSERT_THROW_MESSAGE("Disjoint => should throw.",
                    this->mergeDependencySublists(v1, v2),
                    engine::WTFException);
            CPPUNIT_ASSERT_THROW_MESSAGE("Disjoint => should throw.",
                    this->mergeDependencySublists(v2, v1),
                    engine::WTFException);
        }
        
        {
            vector<shared_ptr<System>> v1 = {s1, s2, s3, s4, s5};
            vector<shared_ptr<System>> v2 = {};
            CPPUNIT_ASSERT_THROW_MESSAGE("Disjoint => should throw.",
                    this->mergeDependencySublists(v1, v2),
                    engine::WTFException);
            CPPUNIT_ASSERT_THROW_MESSAGE("Disjoint => should throw.",
                    this->mergeDependencySublists(v2, v1),
                    engine::WTFException);
        }
        
        {
            vector<shared_ptr<System>> v1 = {};
            vector<shared_ptr<System>> v2 = {};
            CPPUNIT_ASSERT_THROW_MESSAGE("Disjoint => should throw.",
                    this->mergeDependencySublists(v1, v2),
                    engine::WTFException);
            CPPUNIT_ASSERT_THROW_MESSAGE("Disjoint => should throw.",
                    this->mergeDependencySublists(v2, v1),
                    engine::WTFException);
        }
        
        {
            vector<shared_ptr<System>> v1 = {s1, s2, s3, s4, s5};
            vector<shared_ptr<System>> v2 = {s4, s2};
            CPPUNIT_ASSERT_THROW_MESSAGE("Invalid order => should throw.",
                    this->mergeDependencySublists(v1, v2),
                    engine::WTFException);
            CPPUNIT_ASSERT_THROW_MESSAGE("Invalid order => should throw.",
                    this->mergeDependencySublists(v2, v1),
                    engine::WTFException);
        }
    }
    
    void testCriticalComputeRunOrder() {
        auto s5 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystem5>());
        auto ps3 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystemParallel3>());
        auto ps8 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystemParallel8>());
        auto ps6 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystemParallel6>());
        auto s6 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystem6>());
        auto s1 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystem1>());
        auto ps1 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystemParallel1>());
        auto ps5 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystemParallel5>());
        auto s3 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystem3>());
        auto s7 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystem7>());
        auto ps4 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystemParallel4>());
        auto ps2 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystemParallel2>());
        auto s8 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystem8>());
        auto s4 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystem4>());
        auto s2 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystem2>());
        auto ps7 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystemParallel7>());
        
        {
            this->computeRunOrder();
            
            // Find the right list.
            size_t sInd, psInd;
            if(std::find(this->systems[0].begin(), this->systems[0].end(), s1) != this->systems[0].end()) {
                sInd = 0;
                psInd = 1;
            } else {
                sInd = 1;
                psInd = 0;
            }
            
            std::string msg;
            {
                std::stringstream ss;
                ss << "TestSystemIndex " << sInd << "; ParallelTestSystemIndex " << psInd << ";" << std::endl
                    << "Enable order:" << std::endl
                    << this->systemsPreAnalysis << std::endl
                    << "Result:" << std::endl
                    << this->systems << std::endl;
                msg = ss.str();
            }

            {
                std::stringstream ss;
                ss << msg << "Every node must only be contained once.";
                CPPUNIT_ASSERT_MESSAGE(ss.str().c_str(),
                        const_cast<const std::vector<shared_ptr<System>>&>(this->systems[0]).end()
                        == isSet(this->systems[0]));
            }
            {
                std::stringstream ss;
                ss << msg << "Every node must only be contained once.";
                CPPUNIT_ASSERT_MESSAGE(ss.str().c_str(),
                        const_cast<const std::vector<shared_ptr<System>>&>(this->systems[1]).end()
                        == isSet(this->systems[1]));
            }

            {
                std::stringstream ss;
                ss << msg << "Sys1 needs to come before Sys3";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->systems[sInd], s1, s3));
            }
            {
                std::stringstream ss;
                ss << msg << "Sys2 needs to come before Sys3";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->systems[sInd], s2, s3));
            }
            {
                std::stringstream ss;
                ss << msg << "Sys2 needs to come before Sys4";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->systems[sInd], s2, s4));
            }
            {
                std::stringstream ss;
                ss << msg << "Sys3 needs to come before Sys5";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->systems[sInd], s3, s5));
            }
            {
                std::stringstream ss;
                ss << msg << "Sys4 needs to come before Sys5";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->systems[sInd], s4, s5));
            }
            {
                std::stringstream ss;
                ss << msg << "Sys5 needs to come before Sys7";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->systems[sInd], s5, s7));
            }
            {
                std::stringstream ss;
                ss << msg << "Sys5 needs to come before Sys8";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->systems[sInd], s5, s8));
            }
            {
                std::stringstream ss;
                ss << msg << "Sys6 needs to come before Sys8";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->systems[sInd], s6, s8));
            }

            {
                std::stringstream ss;
                ss << msg << "SysPar1 needs to come before SysPar3";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->systems[psInd], ps1, ps3));
            }
            {
                std::stringstream ss;
                ss << msg << "SysPar2 needs to come before SysPar3";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->systems[psInd], ps2, ps3));
            }
            {
                std::stringstream ss;
                ss << msg << "SysPar2 needs to come before SysPar4";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->systems[psInd], ps2, ps4));
            }
            {
                std::stringstream ss;
                ss << msg << "SysPar3 needs to come before SysPar5";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->systems[psInd], ps3, ps5));
            }
            {
                std::stringstream ss;
                ss << msg << "SysPar4 needs to come before SysPar5";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->systems[psInd], ps4, ps5));
            }
            {
                std::stringstream ss;
                ss << msg << "SysPar5 needs to come before SysPar7";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->systems[psInd], ps5, ps7));
            }
            {
                std::stringstream ss;
                ss << msg << "SysPar5 needs to come before SysPar8";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->systems[psInd], ps5, ps8));
            }
            {
                std::stringstream ss;
                ss << msg << "SysPar6 needs to come before SysPar8";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->systems[psInd], ps6, ps8));
            }
        }
    }
    
    void testComputeRunOrder() {
        const size_t NUM_SHUFFLES = 500;
        
        auto s1 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystem1>());
        auto s2 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystem2>());
        auto s3 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystem3>());
        auto s4 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystem4>());
        auto s5 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystem5>());
        auto s6 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystem6>());
        auto s7 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystem7>());
        auto s8 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystem8>());
        
        {
            this->computeRunOrder();

            std::string msg;
            {
                std::stringstream ss;
                ss << "Enable order:" << std::endl
                    << this->systemsPreAnalysis << std::endl
                    << "Result:" << std::endl
                    << this->systems << std::endl;
                msg = ss.str();
            }

            {
                std::stringstream ss;
                ss << msg << "Every node must only be contained once.";
                CPPUNIT_ASSERT_MESSAGE(ss.str().c_str(),
                        const_cast<const std::vector<shared_ptr<System>>&>(this->systems[0]).end()
                        == isSet(this->systems[0]));
            }

            {
                std::stringstream ss;
                ss << msg << "Sys1 needs to come before Sys3";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->systems[0], s1, s3));
            }
            {
                std::stringstream ss;
                ss << msg << "Sys2 needs to come before Sys3";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->systems[0], s2, s3));
            }
            {
                std::stringstream ss;
                ss << msg << "Sys2 needs to come before Sys4";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->systems[0], s2, s4));
            }
            {
                std::stringstream ss;
                ss << msg << "Sys3 needs to come before Sys5";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->systems[0], s3, s5));
            }
            {
                std::stringstream ss;
                ss << msg << "Sys4 needs to come before Sys5";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->systems[0], s4, s5));
            }
            {
                std::stringstream ss;
                ss << msg << "Sys5 needs to come before Sys7";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->systems[0], s5, s7));
            }
            {
                std::stringstream ss;
                ss << msg << "Sys5 needs to come before Sys8";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->systems[0], s5, s8));
            }
            {
                std::stringstream ss;
                ss << msg << "Sys6 needs to come before Sys8";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->systems[0], s6, s8));
            }
        }
        
        this->systems.clear();
        
        auto ps1 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystemParallel1>());
        auto ps2 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystemParallel2>());
        auto ps3 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystemParallel3>());
        auto ps4 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystemParallel4>());
        auto ps5 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystemParallel5>());
        auto ps6 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystemParallel6>());
        auto ps7 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystemParallel7>());
        auto ps8 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystemParallel8>());
        
        {
            this->computeRunOrder();
            // Find the right list.
            size_t sInd, psInd;
            if(std::find(this->systems[0].begin(), this->systems[0].end(), s1) != this->systems[0].end()) {
                sInd = 0;
                psInd = 1;
            } else {
                sInd = 1;
                psInd = 0;
            }
                
            std::string msg;
            {
                std::stringstream ss;
                ss << "TestSystemIndex " << sInd << "; ParallelTestSystemIndex " << psInd << ";" << std::endl
                    << "Enable order:" << std::endl
                    << this->systemsPreAnalysis << std::endl
                    << "Result:" << std::endl
                    << this->systems << std::endl;
                msg = ss.str();
            }

            {
                std::stringstream ss;
                ss << msg << "Every node must only be contained once.";
                CPPUNIT_ASSERT_MESSAGE(ss.str().c_str(),
                        const_cast<const std::vector<shared_ptr<System>>&>(this->systems[0]).end()
                        == isSet(this->systems[0]));
            }
            {
                std::stringstream ss;
                ss << msg << "Every node must only be contained once.";
                CPPUNIT_ASSERT_MESSAGE(ss.str().c_str(),
                        const_cast<const std::vector<shared_ptr<System>>&>(this->systems[1]).end()
                        == isSet(this->systems[1]));
            }

            {
                std::stringstream ss;
                ss << msg << "Sys1 needs to come before Sys3";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->systems[sInd], s1, s3));
            }
            {
                std::stringstream ss;
                ss << msg << "Sys2 needs to come before Sys3";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->systems[sInd], s2, s3));
            }
            {
                std::stringstream ss;
                ss << msg << "Sys2 needs to come before Sys4";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->systems[sInd], s2, s4));
            }
            {
                std::stringstream ss;
                ss << msg << "Sys3 needs to come before Sys5";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->systems[sInd], s3, s5));
            }
            {
                std::stringstream ss;
                ss << msg << "Sys4 needs to come before Sys5";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->systems[sInd], s4, s5));
            }
            {
                std::stringstream ss;
                ss << msg << "Sys5 needs to come before Sys7";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->systems[sInd], s5, s7));
            }
            {
                std::stringstream ss;
                ss << msg << "Sys5 needs to come before Sys8";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->systems[sInd], s5, s8));
            }
            {
                std::stringstream ss;
                ss << msg << "Sys6 needs to come before Sys8";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->systems[sInd], s6, s8));
            }

            {
                std::stringstream ss;
                ss << msg << "SysPar1 needs to come before SysPar3";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->systems[psInd], ps1, ps3));
            }
            {
                std::stringstream ss;
                ss << msg << "SysPar2 needs to come before SysPar3";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->systems[psInd], ps2, ps3));
            }
            {
                std::stringstream ss;
                ss << msg << "SysPar2 needs to come before SysPar4";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->systems[psInd], ps2, ps4));
            }
            {
                std::stringstream ss;
                ss << msg << "SysPar3 needs to come before SysPar5";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->systems[psInd], ps3, ps5));
            }
            {
                std::stringstream ss;
                ss << msg << "SysPar4 needs to come before SysPar5";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->systems[psInd], ps4, ps5));
            }
            {
                std::stringstream ss;
                ss << msg << "SysPar5 needs to come before SysPar7";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->systems[psInd], ps5, ps7));
            }
            {
                std::stringstream ss;
                ss << msg << "SysPar5 needs to come before SysPar8";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->systems[psInd], ps5, ps8));
            }
            {
                std::stringstream ss;
                ss << msg << "SysPar6 needs to come before SysPar8";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->systems[psInd], ps6, ps8));
            }
        }
        
        
        for(size_t i = 0; i < NUM_SHUFFLES; ++i) {
            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(this->systemsPreAnalysis.begin(), this->systemsPreAnalysis.end(), g);

            {
                this->systems.clear();
                this->computeRunOrder();
                
                // Find the right list.
                size_t sInd, psInd;
                if(std::find(this->systems[0].begin(), this->systems[0].end(), s1) != this->systems[0].end()) {
                    sInd = 0;
                    psInd = 1;
                } else {
                    sInd = 1;
                    psInd = 0;
                }
                
                std::string msg;
                {
                    std::stringstream ss;
                    ss << "Iteration " << i << "; TestSystemIndex " << sInd << "; ParallelTestSystemIndex " << psInd << ";" << std::endl
                        << "Enable order:" << std::endl
                        << this->systemsPreAnalysis << std::endl
                        << "Result:" << std::endl
                        << this->systems << std::endl;
                    msg = ss.str();
                }
                
                {
                    std::stringstream ss;
                    ss << msg << "Every node must only be contained once.";
                    CPPUNIT_ASSERT_MESSAGE(ss.str().c_str(),
                            const_cast<const std::vector<shared_ptr<System>>&>(this->systems[0]).end()
                            == isSet(this->systems[0]));
                }
                {
                    std::stringstream ss;
                    ss << msg << "Every node must only be contained once.";
                    CPPUNIT_ASSERT_MESSAGE(ss.str().c_str(),
                            const_cast<const std::vector<shared_ptr<System>>&>(this->systems[1]).end()
                            == isSet(this->systems[1]));
                }
                
                {
                    std::stringstream ss;
                    ss << msg << "Sys1 needs to come before Sys3";
                    CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                            vectorContainsABeforeB(this->systems[sInd], s1, s3));
                }
                {
                    std::stringstream ss;
                    ss << msg << "Sys2 needs to come before Sys3";
                    CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                            vectorContainsABeforeB(this->systems[sInd], s2, s3));
                }
                {
                    std::stringstream ss;
                    ss << msg << "Sys2 needs to come before Sys4";
                    CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                            vectorContainsABeforeB(this->systems[sInd], s2, s4));
                }
                {
                    std::stringstream ss;
                    ss << msg << "Sys3 needs to come before Sys5";
                    CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                            vectorContainsABeforeB(this->systems[sInd], s3, s5));
                }
                {
                    std::stringstream ss;
                    ss << msg << "Sys4 needs to come before Sys5";
                    CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                            vectorContainsABeforeB(this->systems[sInd], s4, s5));
                }
                {
                    std::stringstream ss;
                    ss << msg << "Sys5 needs to come before Sys7";
                    CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                            vectorContainsABeforeB(this->systems[sInd], s5, s7));
                }
                {
                    std::stringstream ss;
                    ss << msg << "Sys5 needs to come before Sys8";
                    CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                            vectorContainsABeforeB(this->systems[sInd], s5, s8));
                }
                {
                    std::stringstream ss;
                    ss << msg << "Sys6 needs to come before Sys8";
                    CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                            vectorContainsABeforeB(this->systems[sInd], s6, s8));
                }
                
                {
                    std::stringstream ss;
                    ss << msg << "SysPar1 needs to come before SysPar3";
                    CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                            vectorContainsABeforeB(this->systems[psInd], ps1, ps3));
                }
                {
                    std::stringstream ss;
                    ss << msg << "SysPar2 needs to come before SysPar3";
                    CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                            vectorContainsABeforeB(this->systems[psInd], ps2, ps3));
                }
                {
                    std::stringstream ss;
                    ss << msg << "SysPar2 needs to come before SysPar4";
                    CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                            vectorContainsABeforeB(this->systems[psInd], ps2, ps4));
                }
                {
                    std::stringstream ss;
                    ss << msg << "SysPar3 needs to come before SysPar5";
                    CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                            vectorContainsABeforeB(this->systems[psInd], ps3, ps5));
                }
                {
                    std::stringstream ss;
                    ss << msg << "SysPar4 needs to come before SysPar5";
                    CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                            vectorContainsABeforeB(this->systems[psInd], ps4, ps5));
                }
                {
                    std::stringstream ss;
                    ss << msg << "SysPar5 needs to come before SysPar7";
                    CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                            vectorContainsABeforeB(this->systems[psInd], ps5, ps7));
                }
                {
                    std::stringstream ss;
                    ss << msg << "SysPar5 needs to come before SysPar8";
                    CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                            vectorContainsABeforeB(this->systems[psInd], ps5, ps8));
                }
                {
                    std::stringstream ss;
                    ss << msg << "SysPar6 needs to come before SysPar8";
                    CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                            vectorContainsABeforeB(this->systems[psInd], ps6, ps8));
                }
            }
        }
    }
};

#endif /* SYSTEMMANAGERTEST_H */

