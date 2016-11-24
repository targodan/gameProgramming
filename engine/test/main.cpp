#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>

#include <easylogging++.h>

#include <iostream>

#include "brief.h"

bool BRIEF_TESTS = false;

INITIALIZE_EASYLOGGINGPP

int main(int argc, char** argv) {
    // Deactivate logging
    el::Configurations defaultConf;
    defaultConf.setToDefault();
    defaultConf.set(el::Level::Global, el::ConfigurationType::ToStandardOutput, "false");
    defaultConf.set(el::Level::Error, el::ConfigurationType::Enabled, "false");
    defaultConf.set(el::Level::Fatal, el::ConfigurationType::Enabled, "false");
    defaultConf.set(el::Level::Info, el::ConfigurationType::Enabled, "false");
    defaultConf.set(el::Level::Trace, el::ConfigurationType::Enabled, "false");
    defaultConf.set(el::Level::Unknown, el::ConfigurationType::Enabled, "false");
    defaultConf.set(el::Level::Verbose, el::ConfigurationType::Enabled, "false");
    defaultConf.set(el::Level::Warning, el::ConfigurationType::Enabled, "false");
    el::Loggers::setDefaultConfigurations(defaultConf, true);
    
    if(argc == 2) {
        if(std::string(argv[1]) == "--brief" || std::string(argv[1]) == "-b") {
            BRIEF_TESTS = true;
        } else {
            std::cout << "Usage: " << argv[0] << " [--brief|-b]" << std::endl;
            return 1;
        }
    }
    
    // Create the event manager and test controller
    CPPUNIT_NS::TestResult controller;

    // Add a listener that colllects test result
    CPPUNIT_NS::TestResultCollector result;
    controller.addListener(&result);

    // Add a listener that print dots as test run.
    CPPUNIT_NS::BriefTestProgressListener progress;
    controller.addListener(&progress);

    // Add the top suite to the test runner
    CPPUNIT_NS::TestRunner runner;
    runner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
    runner.run(controller);

    // Print test in a compiler compatible format.
    CPPUNIT_NS::CompilerOutputter outputter(&result, CPPUNIT_NS::stdCOut());
    outputter.write();
    
    return result.wasSuccessful() ? 0 : -1;
}
