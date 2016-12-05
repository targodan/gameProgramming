#ifndef CONSOLETEST_H
#define CONSOLETEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "util/ostream_helper.h"

#include "IO/Console.h"
#include "util/vector.h"

#include "MockCommands.h"

using engine::IO::Console;
using engine::util::vector;

class ConsoleTest : public CppUnit::TestFixture, public Console {
    CPPUNIT_TEST_SUITE(ConsoleTest);
    CPPUNIT_TEST(testParseLine);
    CPPUNIT_TEST(testExecuteCommand);
    CPPUNIT_TEST_SUITE_END();
    
public:
    ConsoleTest() : Console(32, 512) {}
    
private:
    void testParseLine() {
        {
            auto res = this->parseLine("test_\\t_test");
            vector<std::string> exp = {"test_\t_test"};
            CPPUNIT_ASSERT_EQUAL(exp, res);
        }
        {
            auto res = this->parseLine("test_\\n_test");
            vector<std::string> exp = {"test_\n_test"};
            CPPUNIT_ASSERT_EQUAL(exp, res);
        }
        {
            auto res = this->parseLine("test_\\\\_test");
            vector<std::string> exp = {"test_\\_test"};
            CPPUNIT_ASSERT_EQUAL(exp, res);
        }
        {
            auto res = this->parseLine("test_\\ _test");
            vector<std::string> exp = {"test_ _test"};
            CPPUNIT_ASSERT_EQUAL(exp, res);
        }
        {
            auto res = this->parseLine("test_\\a_test");
            vector<std::string> exp = {"test_\\a_test"};
            CPPUNIT_ASSERT_EQUAL(exp, res);
        }
        {
            auto res = this->parseLine("test1 test2 a   d 3 #3m5.");
            vector<std::string> exp = {"test1", "test2", "a", "d", "3", "#3m5."};
            CPPUNIT_ASSERT_EQUAL(exp, res);
        }
        {
            auto res = this->parseLine("test1 \"test2 a \"  d 3 #3m5.");
            vector<std::string> exp = {"test1", "test2 a ", "d", "3", "#3m5."};
            CPPUNIT_ASSERT_EQUAL(exp, res);
        }
        {
            auto res = this->parseLine("test1 \"test2 \\\"a\\\" \"  d 3 #3m5.");
            vector<std::string> exp = {"test1", "test2 \"a\" ", "d", "3", "#3m5."};
            CPPUNIT_ASSERT_EQUAL(exp, res);
        }
        {
            auto res = this->parseLine("test1 test2 \\\"a\\\"   d 3 #3m5.");
            vector<std::string> exp = {"test1", "test2", "\"a\"", "d", "3", "#3m5."};
            CPPUNIT_ASSERT_EQUAL(exp, res);
        }
    }
    
    void testExecuteCommand() {
        this->registerCommand<MockCommand_ArgsTest>();
        this->registerCommand<MockCommand_OutTest>();
        
        this->linebuffer << "args arg1 arg2 \"some arg with whitespaces\" \"one with extra \\\"quotes\\\"\"";
        this->executeCommandFromLinebuffer();
        CPPUNIT_ASSERT_EQUAL(42, this->waitForCommandExit());
        CPPUNIT_ASSERT_EQUAL(
                std::string("$> args arg1 arg2 \"some arg with whitespaces\" \"one with extra \\\"quotes\\\"\"\n"
                "$> "),
                this->getOutput());
        
        this->linebuffer << "echo arg1 arg2 \"some arg with whitespaces\" \"one with extra \\\"quotes\\\"\"";
        this->executeCommandFromLinebuffer();
        CPPUNIT_ASSERT_EQUAL(666, this->waitForCommandExit());
        CPPUNIT_ASSERT_EQUAL(
                std::string("$> args arg1 arg2 \"some arg with whitespaces\" \"one with extra \\\"quotes\\\"\"\n"
                "$> echo arg1 arg2 \"some arg with whitespaces\" \"one with extra \\\"quotes\\\"\"\n"
                "arg1 arg2 some arg with whitespaces one with extra \"quotes\"\n"
                "$> "),
                this->getOutput());
    }
};

#endif /* CONSOLETEST_H */

