#ifndef SYSTEMMANAGERTEST_H
#define SYSTEMMANAGERTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <memory>
#include <random>
#include <algorithm>
#include <easylogging++.h>
#include <boost/filesystem.hpp>

#include "../macros.h"

#include "WTFException.h"
#include "util/ostream_helper.h"
#include "ECS/DependencyException.h"
#include "ECS/SystemManager.h"

#include "SystemImplementations.h"

using namespace engine::ECS;
using std::shared_ptr;

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

EntityManager _em;

class SystemManagerTest : public CPPUNIT_NS::TestFixture, public SystemManager {
    CPPUNIT_TEST_SUITE(SystemManagerTest);
    CPPUNIT_TEST(testBuildDependencyGraph);
    CPPUNIT_TEST(testIsGraphCircular_true);
    CPPUNIT_TEST(testIsGraphCircular_false);
    CPPUNIT_TEST(testAssignLayers);
    CPPUNIT_TEST(testSetupStop);
    CPPUNIT_TEST(testRun);
    CPPUNIT_TEST(testThrow);
    CPPUNIT_TEST_SUITE_END();

public:
    SystemManagerTest() : SystemManager(_em) {}
    
    void tearDown() override {
        this->enabledSystems.clear();
        this->dependencyTree.clear();
    }
    
private:
    template<class T>
    bool vectorContainsABeforeB(std::vector<T> v, const T& A, const T& B) {
        auto itA = std::find(v.begin(), v.end(), A);
        auto itB = std::find(v.begin(), v.end(), B);
        if(itA == v.end() || itB == v.end()) {
            return false;
        }
        return itA < itB;
    }

    bool vectorContainsABeforeB(std::vector<std::shared_ptr<SystemManager::SystemNode>> v,
            const std::shared_ptr<System>& A,
            const std::shared_ptr<System>& B) {
        auto itA = std::find_if(v.begin(), v.end(), [&](auto n) {return n->system == A;});
        auto itB = std::find_if(v.begin(), v.end(), [&](auto n) {return n->system == B;});
        if(itA == v.end() || itB == v.end()) {
            return false;
        }
        return itA < itB;
    }
    
    bool testLayerSorting() const {
        size_t layer = SIZE_MAX;
        for(auto& n : this->dependencyTree) {
            if(layer == n->layer) {
                continue;
            }
            if(layer > n->layer) {
                layer = n->layer;
            }
            if(layer < n->layer) {
                return false;
            }
        }
        return true;
    }
    
    bool testLayerWidth() const {
        size_t layer = SIZE_MAX;
        size_t layerSize = 0;
        for(auto& n : this->dependencyTree) {
            if(layer == n->layer) {
                ++layerSize;
                if(layerSize > this->numThreads) {
                    return false;
                }
                continue;
            }
            if(layer > n->layer) {
                layer = n->layer;
                layerSize = 0;
            }
        }
        return true;
    }
    
    void testSetupStop() {
        this->enableSystem<TestSystem1>();
        this->enableSystem<TestSystem2>();
        this->enableSystem<TestSystem3>();
        this->enableSystem<TestSystem4>();
        this->enableSystem<TestSystem5>();
        this->enableSystem<TestSystem6>();
        this->enableSystem<TestSystem7>();
        this->enableSystem<TestSystem8>();

        this->enableSystem<TestSystemParallel1>();
        this->enableSystem<TestSystemParallel2>();
        this->enableSystem<TestSystemParallel3>();
        this->enableSystem<TestSystemParallel4>();
        this->enableSystem<TestSystemParallel5>();
        this->enableSystem<TestSystemParallel6>();
        this->enableSystem<TestSystemParallel7>();
        this->enableSystem<TestSystemParallel8>();
        
        CPPUNIT_ASSERT_THROW_MESSAGE("Not setup => should throw.", this->stop(), engine::WTFException);
        CPPUNIT_ASSERT_NO_THROW(this->setup());
        CPPUNIT_ASSERT_THROW_MESSAGE("Already setup => should throw.", this->setup(), engine::WTFException);
        CPPUNIT_ASSERT_NO_THROW(this->stop());
        CPPUNIT_ASSERT_THROW_MESSAGE("Already stopped => should throw.", this->stop(), engine::WTFException);
    }
    
    void testRun() {
        const size_t NUM_RUNS = 200;
        
        auto s1 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystem1>());
        auto s2 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystem2>());
        auto s3 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystem3>());
        auto s4 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystem4>());
        auto s5 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystem5>());
        auto s6 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystem6>());
        auto s7 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystem7>());
        auto s8 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystem8>());
        
        auto ps1 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystemParallel1>());
        auto ps2 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystemParallel2>());
        auto ps3 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystemParallel3>());
        auto ps4 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystemParallel4>());
        auto ps5 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystemParallel5>());
        auto ps6 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystemParallel6>());
        auto ps7 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystemParallel7>());
        auto ps8 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystemParallel8>());
        
        this->setup();
        
        vector<std::string> executionOrder;
        executionOrder.reserve(this->enabledSystems.size());
        for(size_t i = 0; i < NUM_RUNS; ++i) {
            this->runParallel(SystemType::UPDATE, 0);
            
            executionOrder.clear();
            while(!__executionQueue.empty()) {
                executionOrder.push_back(__executionQueue.pop());
            }
            
            std::string msg;
            
            {
                std::stringstream ss;
                ss << msg << "Every node must only be contained once.";
                CPPUNIT_ASSERT_MESSAGE(ss.str().c_str(),
                        executionOrder.cend()
                        == isSet(executionOrder));
            }

            {
                std::stringstream ss;
                ss << msg << "Sys1 needs to come before Sys3";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(executionOrder, s1->getSystemName(), s3->getSystemName()));
            }
            {
                std::stringstream ss;
                ss << msg << "Sys2 needs to come before Sys3";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(executionOrder, s2->getSystemName(), s3->getSystemName()));
            }
            {
                std::stringstream ss;
                ss << msg << "Sys2 needs to come before Sys4";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(executionOrder, s2->getSystemName(), s4->getSystemName()));
            }
            {
                std::stringstream ss;
                ss << msg << "Sys3 needs to come before Sys5";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(executionOrder, s3->getSystemName(), s5->getSystemName()));
            }
            {
                std::stringstream ss;
                ss << msg << "Sys4 needs to come before Sys5";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(executionOrder, s4->getSystemName(), s5->getSystemName()));
            }
            {
                std::stringstream ss;
                ss << msg << "Sys5 needs to come before Sys7";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(executionOrder, s5->getSystemName(), s7->getSystemName()));
            }
            {
                std::stringstream ss;
                ss << msg << "Sys5 needs to come before Sys8";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(executionOrder, s5->getSystemName(), s8->getSystemName()));
            }
            {
                std::stringstream ss;
                ss << msg << "Sys6 needs to come before Sys8";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(executionOrder, s6->getSystemName(), s8->getSystemName()));
            }

            {
                std::stringstream ss;
                ss << msg << "SysPar1 needs to come before SysPar3";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(executionOrder, ps1->getSystemName(), ps3->getSystemName()));
            }
            {
                std::stringstream ss;
                ss << msg << "SysPar2 needs to come before SysPar3";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(executionOrder, ps2->getSystemName(), ps3->getSystemName()));
            }
            {
                std::stringstream ss;
                ss << msg << "SysPar2 needs to come before SysPar4";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(executionOrder, ps2->getSystemName(), ps4->getSystemName()));
            }
            {
                std::stringstream ss;
                ss << msg << "SysPar3 needs to come before SysPar5";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(executionOrder, ps3->getSystemName(), ps5->getSystemName()));
            }
            {
                std::stringstream ss;
                ss << msg << "SysPar4 needs to come before SysPar5";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(executionOrder, ps4->getSystemName(), ps5->getSystemName()));
            }
            {
                std::stringstream ss;
                ss << msg << "SysPar5 needs to come before SysPar7";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(executionOrder, ps5->getSystemName(), ps7->getSystemName()));
            }
            {
                std::stringstream ss;
                ss << msg << "SysPar5 needs to come before SysPar8";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(executionOrder, ps5->getSystemName(), ps8->getSystemName()));
            }
            {
                std::stringstream ss;
                ss << msg << "SysPar6 needs to come before SysPar8";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(executionOrder, ps6->getSystemName(), ps8->getSystemName()));
            }
        }
        
        this->stop();
    }
    
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
        
        roots.clear();
    }
    
    void testIsGraphCircular_true() {
        this->enableSystem<LoopTest1>();
        this->enableSystem<LoopTest2>();
        this->enableSystem<LoopTest3>();
        
        auto roots = this->buildDependencyGraph();
        CPPUNIT_ASSERT_EQUAL(true, this->isGraphCircular(roots));
        roots.clear();
        this->clearCircularGraph();
        this->dependencyTree.clear();
        
        this->enableSystem<LoopTest0>();
        
        roots = this->buildDependencyGraph();
        CPPUNIT_ASSERT_EQUAL(true, this->isGraphCircular(roots));
        roots.clear();
        this->clearCircularGraph();
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
        roots.clear();
    }
    
    void testAssignLayers() {
        const size_t NUM_SHUFFLES = 2;
        const size_t MAX_NUM_THREADS = 8;
        
        auto s1 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystem1>());
        auto s2 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystem2>());
        auto s3 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystem3>());
        auto s4 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystem4>());
        auto s5 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystem5>());
        auto s6 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystem6>());
        auto s7 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystem7>());
        auto s8 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystem8>());
        
        {
            this->buildDependencyGraph();
            this->assignLayers();

            std::string msg;
            {
                std::stringstream ss;
                ss << "Enable order:" << std::endl
                    << this->enabledSystems << std::endl
                    << "Result:" << std::endl
                    << this->dependencyTree << std::endl;
                msg = ss.str();
            }

            {
                std::stringstream ss;
                ss << msg << "The layers must be sorted in descending order.";
                CPPUNIT_ASSERT_MESSAGE(ss.str().c_str(),
                        this->testLayerSorting());
            }

            {
                std::stringstream ss;
                ss << msg << "The layers must not be wider than " << this->numThreads << ".";
                CPPUNIT_ASSERT_MESSAGE(ss.str().c_str(),
                        this->testLayerWidth());
            }

            {
                std::stringstream ss;
                ss << msg << "Every node must only be contained once.";
                CPPUNIT_ASSERT_MESSAGE(ss.str().c_str(),
                        this->dependencyTree.cend()
                        == isSet(this->dependencyTree));
            }

            {
                std::stringstream ss;
                ss << msg << "Sys1 needs to come before Sys3";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->dependencyTree, s1, s3));
            }
            {
                std::stringstream ss;
                ss << msg << "Sys2 needs to come before Sys3";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->dependencyTree, s2, s3));
            }
            {
                std::stringstream ss;
                ss << msg << "Sys2 needs to come before Sys4";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->dependencyTree, s2, s4));
            }
            {
                std::stringstream ss;
                ss << msg << "Sys3 needs to come before Sys5";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->dependencyTree, s3, s5));
            }
            {
                std::stringstream ss;
                ss << msg << "Sys4 needs to come before Sys5";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->dependencyTree, s4, s5));
            }
            {
                std::stringstream ss;
                ss << msg << "Sys5 needs to come before Sys7";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->dependencyTree, s5, s7));
            }
            {
                std::stringstream ss;
                ss << msg << "Sys5 needs to come before Sys8";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->dependencyTree, s5, s8));
            }
            {
                std::stringstream ss;
                ss << msg << "Sys6 needs to come before Sys8";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->dependencyTree, s6, s8));
            }
        }
        
        auto ps1 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystemParallel1>());
        auto ps2 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystemParallel2>());
        auto ps3 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystemParallel3>());
        auto ps4 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystemParallel4>());
        auto ps5 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystemParallel5>());
        auto ps6 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystemParallel6>());
        auto ps7 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystemParallel7>());
        auto ps8 = static_cast<shared_ptr<System>>(this->enableSystem<TestSystemParallel8>());
        
        {
            this->dependencyTree.clear();
            
            this->buildDependencyGraph();
            this->assignLayers();
                
            std::string msg;
            {
                std::stringstream ss;
                ss << "Enable order:" << std::endl
                    << this->enabledSystems << std::endl
                    << "Result:" << std::endl
                    << this->dependencyTree << std::endl;
                msg = ss.str();
            }

            {
                std::stringstream ss;
                ss << msg << "The layers must be sorted in descending order.";
                CPPUNIT_ASSERT_MESSAGE(ss.str().c_str(),
                        this->testLayerSorting());
            }

            {
                std::stringstream ss;
                ss << msg << "The layers must not be wider than " << this->numThreads << ".";
                CPPUNIT_ASSERT_MESSAGE(ss.str().c_str(),
                        this->testLayerWidth());
            }

            {
                std::stringstream ss;
                ss << msg << "Every node must only be contained once.";
                CPPUNIT_ASSERT_MESSAGE(ss.str().c_str(),
                        this->dependencyTree.cend()
                        == isSet(this->dependencyTree));
            }

            {
                std::stringstream ss;
                ss << msg << "Sys1 needs to come before Sys3";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->dependencyTree, s1, s3));
            }
            {
                std::stringstream ss;
                ss << msg << "Sys2 needs to come before Sys3";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->dependencyTree, s2, s3));
            }
            {
                std::stringstream ss;
                ss << msg << "Sys2 needs to come before Sys4";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->dependencyTree, s2, s4));
            }
            {
                std::stringstream ss;
                ss << msg << "Sys3 needs to come before Sys5";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->dependencyTree, s3, s5));
            }
            {
                std::stringstream ss;
                ss << msg << "Sys4 needs to come before Sys5";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->dependencyTree, s4, s5));
            }
            {
                std::stringstream ss;
                ss << msg << "Sys5 needs to come before Sys7";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->dependencyTree, s5, s7));
            }
            {
                std::stringstream ss;
                ss << msg << "Sys5 needs to come before Sys8";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->dependencyTree, s5, s8));
            }
            {
                std::stringstream ss;
                ss << msg << "Sys6 needs to come before Sys8";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->dependencyTree, s6, s8));
            }

            {
                std::stringstream ss;
                ss << msg << "SysPar1 needs to come before SysPar3";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->dependencyTree, ps1, ps3));
            }
            {
                std::stringstream ss;
                ss << msg << "SysPar2 needs to come before SysPar3";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->dependencyTree, ps2, ps3));
            }
            {
                std::stringstream ss;
                ss << msg << "SysPar2 needs to come before SysPar4";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->dependencyTree, ps2, ps4));
            }
            {
                std::stringstream ss;
                ss << msg << "SysPar3 needs to come before SysPar5";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->dependencyTree, ps3, ps5));
            }
            {
                std::stringstream ss;
                ss << msg << "SysPar4 needs to come before SysPar5";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->dependencyTree, ps4, ps5));
            }
            {
                std::stringstream ss;
                ss << msg << "SysPar5 needs to come before SysPar7";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->dependencyTree, ps5, ps7));
            }
            {
                std::stringstream ss;
                ss << msg << "SysPar5 needs to come before SysPar8";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->dependencyTree, ps5, ps8));
            }
            {
                std::stringstream ss;
                ss << msg << "SysPar6 needs to come before SysPar8";
                CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                        vectorContainsABeforeB(this->dependencyTree, ps6, ps8));
            }
        }
        
        for(size_t threads = 1; threads < MAX_NUM_THREADS; ++threads) {
            this->setNumberOfThreads(threads);
            std::random_device rd;
            std::mt19937 g(rd());
            for(size_t i = 0; i < NUM_SHUFFLES; ++i) {
                std::shuffle(this->enabledSystems.begin(), this->enabledSystems.end(), g);

                {
                    this->dependencyTree.clear();

                    this->buildDependencyGraph();
                    this->assignLayers();

                    std::string msg;
                    {
                        std::stringstream ss;
                        ss << "Iteration " << i << ";" << std::endl
                            << "Enable order:" << std::endl
                            << this->enabledSystems << std::endl
                            << "Result:" << std::endl
                            << this->dependencyTree << std::endl;
                        msg = ss.str();
                    }

                    {
                        std::stringstream ss;
                        ss << msg << "The layers must be sorted in descending order.";
                        CPPUNIT_ASSERT_MESSAGE(ss.str().c_str(),
                                this->testLayerSorting());
                    }

                    {
                        std::stringstream ss;
                        ss << msg << "The layers must not be wider than " << this->numThreads << ".";
                        CPPUNIT_ASSERT_MESSAGE(ss.str().c_str(),
                                this->testLayerWidth());
                    }

                    {
                        std::stringstream ss;
                        ss << msg << "Every node must only be contained once.";
                        CPPUNIT_ASSERT_MESSAGE(ss.str().c_str(),
                                this->dependencyTree.cend()
                                == isSet(this->dependencyTree));
                    }
                    {
                        std::stringstream ss;
                        ss << msg << "Every node must only be contained once.";
                        CPPUNIT_ASSERT_MESSAGE(ss.str().c_str(),
                                this->dependencyTree.cend()
                                == isSet(this->dependencyTree));
                    }

                    {
                        std::stringstream ss;
                        ss << msg << "Sys1 needs to come before Sys3";
                        CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                                vectorContainsABeforeB(this->dependencyTree, s1, s3));
                    }
                    {
                        std::stringstream ss;
                        ss << msg << "Sys2 needs to come before Sys3";
                        CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                                vectorContainsABeforeB(this->dependencyTree, s2, s3));
                    }
                    {
                        std::stringstream ss;
                        ss << msg << "Sys2 needs to come before Sys4";
                        CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                                vectorContainsABeforeB(this->dependencyTree, s2, s4));
                    }
                    {
                        std::stringstream ss;
                        ss << msg << "Sys3 needs to come before Sys5";
                        CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                                vectorContainsABeforeB(this->dependencyTree, s3, s5));
                    }
                    {
                        std::stringstream ss;
                        ss << msg << "Sys4 needs to come before Sys5";
                        CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                                vectorContainsABeforeB(this->dependencyTree, s4, s5));
                    }
                    {
                        std::stringstream ss;
                        ss << msg << "Sys5 needs to come before Sys7";
                        CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                                vectorContainsABeforeB(this->dependencyTree, s5, s7));
                    }
                    {
                        std::stringstream ss;
                        ss << msg << "Sys5 needs to come before Sys8";
                        CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                                vectorContainsABeforeB(this->dependencyTree, s5, s8));
                    }
                    {
                        std::stringstream ss;
                        ss << msg << "Sys6 needs to come before Sys8";
                        CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                                vectorContainsABeforeB(this->dependencyTree, s6, s8));
                    }

                    {
                        std::stringstream ss;
                        ss << msg << "SysPar1 needs to come before SysPar3";
                        CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                                vectorContainsABeforeB(this->dependencyTree, ps1, ps3));
                    }
                    {
                        std::stringstream ss;
                        ss << msg << "SysPar2 needs to come before SysPar3";
                        CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                                vectorContainsABeforeB(this->dependencyTree, ps2, ps3));
                    }
                    {
                        std::stringstream ss;
                        ss << msg << "SysPar2 needs to come before SysPar4";
                        CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                                vectorContainsABeforeB(this->dependencyTree, ps2, ps4));
                    }
                    {
                        std::stringstream ss;
                        ss << msg << "SysPar3 needs to come before SysPar5";
                        CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                                vectorContainsABeforeB(this->dependencyTree, ps3, ps5));
                    }
                    {
                        std::stringstream ss;
                        ss << msg << "SysPar4 needs to come before SysPar5";
                        CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                                vectorContainsABeforeB(this->dependencyTree, ps4, ps5));
                    }
                    {
                        std::stringstream ss;
                        ss << msg << "SysPar5 needs to come before SysPar7";
                        CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                                vectorContainsABeforeB(this->dependencyTree, ps5, ps7));
                    }
                    {
                        std::stringstream ss;
                        ss << msg << "SysPar5 needs to come before SysPar8";
                        CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                                vectorContainsABeforeB(this->dependencyTree, ps5, ps8));
                    }
                    {
                        std::stringstream ss;
                        ss << msg << "SysPar6 needs to come before SysPar8";
                        CPPUNIT_ASSERT_EQUAL_MESSAGE(ss.str().c_str(), true,
                                vectorContainsABeforeB(this->dependencyTree, ps6, ps8));
                    }
                }
            }
        }
    }
    
    void testThrow() {
        auto s = this->enableSystem<ThrowTest>();
        this->setup();
        
        auto temp = boost::filesystem::temp_directory_path();
        temp.append("enginetest.log");
        
        auto logger = el::Loggers::getLogger("default", false);
        auto origConf = logger->configurations();
        auto conf = *origConf;
        conf.set(el::Level::Fatal, el::ConfigurationType::Enabled, "true");
        conf.set(el::Level::Fatal, el::ConfigurationType::Filename, temp.c_str());
        el::Loggers::reconfigureLogger(logger, conf);
        bool hadFlag = el::Loggers::hasFlag(el::LoggingFlag::DisableApplicationAbortOnFatalLog);
        el::Loggers::addFlag(el::LoggingFlag::DisableApplicationAbortOnFatalLog);
        
        this->runParallel(SystemType::UPDATE, 0);
        logger->flush();
        
        std::ifstream log(temp.c_str());
        std::string logcontent;
        log.seekg(0, std::ios::end);
        logcontent.reserve(log.tellg());
        log.seekg(0, std::ios::beg);
        logcontent.assign((std::istreambuf_iterator<char>(log)),
                            std::istreambuf_iterator<char>());
        
        std::stringstream ss;
        ss << "The exception should have been logged to \"" << temp.c_str() << "\".";
        CPPUNIT_ASSERT_MESSAGE(ss.str(), logcontent.find("This is a test exception.") != std::string::npos);
        
        el::Loggers::reconfigureLogger(logger, *origConf);
        if(!hadFlag) {
            el::Loggers::removeFlag(el::LoggingFlag::DisableApplicationAbortOnFatalLog);
        }
    }
};

#endif /* SYSTEMMANAGERTEST_H */

