#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <iostream>
#include "out.h"

int main(int argc, char** argv) {
    CppUnit::TextUi::TestRunner runner;
    CppUnit::TestFactoryRegistry& registry = CppUnit::TestFactoryRegistry::getRegistry();
    runner.addTest(registry.makeTest());
    bool success = runner.run();
    
    std::cout << "----------------- Test Output -----------------" << std::endl
            << out.str() << std::endl;
    
    return success ? 0 : -1;
}
