#include "SystemImplementations.h"

engine::util::BlockingQueue<std::string> __executionQueue;

DEFINE_SYSTEM_ID(TestSystem1);
DEFINE_SYSTEM_ID(TestSystem2);
DEFINE_SYSTEM_ID(TestSystem3);
DEFINE_SYSTEM_ID(TestSystem4);
DEFINE_SYSTEM_ID(TestSystem5);
DEFINE_SYSTEM_ID(TestSystem6);
DEFINE_SYSTEM_ID(TestSystem7);
DEFINE_SYSTEM_ID(TestSystem8);

DEFINE_SYSTEM_ID(TestSystemParallel1);
DEFINE_SYSTEM_ID(TestSystemParallel2);
DEFINE_SYSTEM_ID(TestSystemParallel3);
DEFINE_SYSTEM_ID(TestSystemParallel4);
DEFINE_SYSTEM_ID(TestSystemParallel5);
DEFINE_SYSTEM_ID(TestSystemParallel6);
DEFINE_SYSTEM_ID(TestSystemParallel7);
DEFINE_SYSTEM_ID(TestSystemParallel8);

DEFINE_SYSTEM_ID(LoopTest0);
DEFINE_SYSTEM_ID(LoopTest1);
DEFINE_SYSTEM_ID(LoopTest2);
DEFINE_SYSTEM_ID(LoopTest3);

ECS_REGISTER_SYSTEM(TestSystem1);
ECS_REGISTER_SYSTEM(TestSystem2);
ECS_REGISTER_SYSTEM(TestSystem3);
ECS_REGISTER_SYSTEM(TestSystem4);
ECS_REGISTER_SYSTEM(TestSystem5);
ECS_REGISTER_SYSTEM(TestSystem6);
ECS_REGISTER_SYSTEM(TestSystem7);
ECS_REGISTER_SYSTEM(TestSystem8);

ECS_REGISTER_SYSTEM(TestSystemParallel1);
ECS_REGISTER_SYSTEM(TestSystemParallel2);
ECS_REGISTER_SYSTEM(TestSystemParallel3);
ECS_REGISTER_SYSTEM(TestSystemParallel4);
ECS_REGISTER_SYSTEM(TestSystemParallel5);
ECS_REGISTER_SYSTEM(TestSystemParallel6);
ECS_REGISTER_SYSTEM(TestSystemParallel7);
ECS_REGISTER_SYSTEM(TestSystemParallel8);

ECS_REGISTER_SYSTEM(LoopTest0);
ECS_REGISTER_SYSTEM(LoopTest1);
ECS_REGISTER_SYSTEM(LoopTest2);
ECS_REGISTER_SYSTEM(LoopTest3);

Array<systemId_t> LoopTest3::getDependencies() const {
    return {LoopTest2::systemTypeId()};
}

DEFINE_SYSTEM_ID(ThrowTest);
ECS_REGISTER_SYSTEM(ThrowTest);