#ifndef SYSTEMIMPLEMENTATIONS_H
#define SYSTEMIMPLEMENTATIONS_H

#include "ECS/System.h"
#include "ECS/SystemRegisterer.h"
#include "util/BlockingQueue.h"

#include <iostream>

using namespace engine::ECS;

extern engine::util::BlockingQueue<std::string> __executionQueue;

#define CREATE_TEST_SYSTEM(name, deps, optDeps) \
        class name : public System { \
        private: \
            static systemId_t systemId; \
        public: \
            void run(EntityManager& em) override { \
                __executionQueue.push(this->getSystemName()); \
            } \
            Array<systemId_t> getDependencies() const override { \
                return deps; \
            } \
            Array<systemId_t> getOptionalDependencies() const override { \
                return optDeps; \
            } \
            std::string getSystemName() const override { \
                return #name; \
            } \
            systemId_t getSystemTypeId() const override { \
                return name::systemId; \
            } \
            static systemId_t systemTypeId() { \
                return name::systemId; \
            } \
            static void setSystemTypeId(systemId_t id) { \
                name::systemId = id; \
            } \
        }

#define DEFINE_SYSTEM_ID(name) systemId_t name::systemId = 0

#define DEPENDENCIES(...) {__VA_ARGS__}

/*
 * Now building up this tree:
 * 
 *  TestSystem1_..............................._ TestSystem7
 *              \_ TestSystem3                /
 *              /             \_ (TestSystem5)
 *  TestSystem2               /               \_ TestSystem8
 *              \_ TestSystem4                /
 *                                 TestSystem6
 * 
 * Nodes with brackets can be removed.
 */

CREATE_TEST_SYSTEM(TestSystem1, {}, {});
CREATE_TEST_SYSTEM(TestSystem2, {}, {});
CREATE_TEST_SYSTEM(TestSystem3, DEPENDENCIES(TestSystem1::systemTypeId(), TestSystem2::systemTypeId()), {});
CREATE_TEST_SYSTEM(TestSystem4, {TestSystem2::systemTypeId()}, {});
CREATE_TEST_SYSTEM(TestSystem5, DEPENDENCIES(TestSystem3::systemTypeId(), TestSystem4::systemTypeId()), {});
CREATE_TEST_SYSTEM(TestSystem6, {}, {});
CREATE_TEST_SYSTEM(TestSystem7, {}, DEPENDENCIES(TestSystem1::systemTypeId(), TestSystem5::systemTypeId()));
CREATE_TEST_SYSTEM(TestSystem8, {TestSystem6::systemTypeId()}, {TestSystem5::systemTypeId()});

CREATE_TEST_SYSTEM(TestSystemParallel1, {}, {});
CREATE_TEST_SYSTEM(TestSystemParallel2, {}, {});
CREATE_TEST_SYSTEM(TestSystemParallel3, DEPENDENCIES(TestSystemParallel1::systemTypeId(), TestSystemParallel2::systemTypeId()), {});
CREATE_TEST_SYSTEM(TestSystemParallel4, {TestSystemParallel2::systemTypeId()}, {});
CREATE_TEST_SYSTEM(TestSystemParallel5, DEPENDENCIES(TestSystemParallel3::systemTypeId(), TestSystemParallel4::systemTypeId()), {});
CREATE_TEST_SYSTEM(TestSystemParallel6, {}, {});
CREATE_TEST_SYSTEM(TestSystemParallel7, {}, DEPENDENCIES(TestSystemParallel1::systemTypeId(), TestSystemParallel5::systemTypeId()));
CREATE_TEST_SYSTEM(TestSystemParallel8, {TestSystemParallel6::systemTypeId()}, {TestSystemParallel5::systemTypeId()});


class LoopTest3 : public System {
private:
    static systemId_t systemId;
public:
    void run(EntityManager& em) override {}
    Array<systemId_t> getDependencies() const override;
    Array<systemId_t> getOptionalDependencies() const override {
        return {};
    }
    std::string getSystemName() const override {
        return "LoopTest3";
    }
    systemId_t getSystemTypeId() const override {
        return LoopTest3::systemId;
    }
    static systemId_t systemTypeId() {
        return LoopTest3::systemId;
    }
    static void setSystemTypeId(systemId_t id) {
        LoopTest3::systemId = id;
    }
};
CREATE_TEST_SYSTEM(LoopTest0, {}, {});
CREATE_TEST_SYSTEM(LoopTest1, {LoopTest3::systemTypeId()}, {LoopTest0::systemTypeId()});
CREATE_TEST_SYSTEM(LoopTest2, {LoopTest1::systemTypeId()}, {});

#endif /* SYSTEMIMPLEMENTATIONS_H */

