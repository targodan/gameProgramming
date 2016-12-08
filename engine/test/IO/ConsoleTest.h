#ifndef CONSOLETEST_H
#define CONSOLETEST_H

#include <cppunit/extensions/HelperMacros.h>

#include <initializer_list>

#include "util/ostream_helper.h"

#include "IO/Console.h"
#include "IO/Keys.h"
#include "util/vector.h"

#include "MockCommands.h"

using engine::IO::Console;
using engine::IO::Keys;
using engine::util::vector;

class ConsoleTest : public CppUnit::TestFixture, public Console {
    CPPUNIT_TEST_SUITE(ConsoleTest);
    CPPUNIT_TEST(testParseLine);
    CPPUNIT_TEST(testExecuteCommand);
    CPPUNIT_TEST(testReceiveKeypress_simple);
    CPPUNIT_TEST(testReceiveKeypress_arrowsLeftRight);
    CPPUNIT_TEST(testReceiveKeypress_backspaceDel);
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
    
    void sendKeys(std::initializer_list<std::string> texts) {
        for(auto& t : texts) {
            for(char c : t) {
                this->receiveKeypress(c);
            }
        }
    }
    
    std::string str(char c) {
        std::string s;
        s.append(&c, 1);
        return s;
    }
    
    void testReceiveKeypress_simple() {
        this->sendKeys({"this is a simple test"});
        CPPUNIT_ASSERT_EQUAL(std::string("this is a simple test"), this->linebuffer.str());
        
        this->linebuffer.str("");
    }
    
    void testReceiveKeypress_arrowsLeftRight() {
        this->sendKeys({"this is a simple test", str(Keys::ARROW_LEFT), str(Keys::ARROW_LEFT), str(Keys::ARROW_LEFT), str(Keys::ARROW_LEFT), str(Keys::ARROW_LEFT), str(Keys::ARROW_LEFT), str(Keys::ARROW_LEFT), str(Keys::ARROW_LEFT), str(Keys::ARROW_LEFT), str(Keys::ARROW_LEFT), str(Keys::ARROW_LEFT),
        "not so "});
        CPPUNIT_ASSERT_EQUAL(std::string("this is a not so simple test"), this->linebuffer.str());
        
        this->linebuffer.str("");
        
        this->sendKeys({"this is a simple test", str(Keys::ARROW_LEFT), str(Keys::ARROW_RIGHT), str(Keys::ARROW_RIGHT),
                " as well"});
        CPPUNIT_ASSERT_EQUAL(std::string("this is a simple test as well"), this->linebuffer.str());
        
        this->linebuffer.str("");
        
        this->sendKeys({str(Keys::ARROW_LEFT), str(Keys::ARROW_LEFT), str(Keys::ARROW_LEFT)});
        CPPUNIT_ASSERT_EQUAL(std::string(""), this->linebuffer.str());
        
        this->linebuffer.str("");
        
        this->sendKeys({str(Keys::ARROW_RIGHT), str(Keys::ARROW_RIGHT), str(Keys::ARROW_RIGHT)});
        CPPUNIT_ASSERT_EQUAL(std::string(""), this->linebuffer.str());
    }
    
    void testReceiveKeypress_backspaceDel() {
        this->sendKeys({"this is a simple test", str(Keys::BACKSPACE), str(Keys::BACKSPACE), str(Keys::BACKSPACE), str(Keys::BACKSPACE), str(Keys::BACKSPACE)});
        CPPUNIT_ASSERT_EQUAL(std::string("this is a simple"), this->linebuffer.str());
        
        this->linebuffer.str("");
        
        this->sendKeys({"this is a simple test", str(Keys::BACKSPACE), str(Keys::BACKSPACE), str(Keys::BACKSPACE), str(Keys::BACKSPACE), str(Keys::BACKSPACE),
                " cheese"});
        CPPUNIT_ASSERT_EQUAL(std::string("this is a simple cheese"), this->linebuffer.str());
        
        this->linebuffer.str("");
        
        this->sendKeys({"this is a simple test", str(Keys::ARROW_LEFT), str(Keys::ARROW_LEFT), str(Keys::ARROW_LEFT), str(Keys::ARROW_LEFT), str(Keys::ARROW_LEFT), str(Keys::BACKSPACE), str(Keys::BACKSPACE), str(Keys::BACKSPACE), str(Keys::BACKSPACE), str(Keys::BACKSPACE), str(Keys::BACKSPACE),
                "cheese"});
        CPPUNIT_ASSERT_EQUAL(std::string("this is a cheese test"), this->linebuffer.str());
        
        this->linebuffer.str("");
        
        this->sendKeys({"this is a simple test", str(Keys::ARROW_LEFT), str(Keys::ARROW_LEFT), str(Keys::ARROW_LEFT), str(Keys::ARROW_LEFT), str(Keys::ARROW_LEFT), str(Keys::DEL), str(Keys::DEL), str(Keys::DEL), str(Keys::DEL), str(Keys::DEL),
                " cheese"});
        CPPUNIT_ASSERT_EQUAL(std::string("this is a simple cheese"), this->linebuffer.str());
        
        this->linebuffer.str("");
        
        this->sendKeys({str(Keys::BACKSPACE), str(Keys::BACKSPACE), str(Keys::BACKSPACE)});
        CPPUNIT_ASSERT_EQUAL(std::string(""), this->linebuffer.str());
        
        this->linebuffer.str("");
        
        this->sendKeys({str(Keys::DEL), str(Keys::DEL), str(Keys::DEL)});
        CPPUNIT_ASSERT_EQUAL(std::string(""), this->linebuffer.str());
    }
};

#endif /* CONSOLETEST_H */

