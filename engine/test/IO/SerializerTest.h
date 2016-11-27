#ifndef SERIALIZABLETEST_H
#define SERIALIZABLETEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "IO/SerializerFactory.h"

#include "serializer_test.pb.h"

using engine::IO::SerializerFactory;

class SerializerTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(SerializerTest);
    CPPUNIT_TEST(testSerializeHumanReadable);
    CPPUNIT_TEST(testDeserializeHumanReadable);
    CPPUNIT_TEST(testSerializeDeserializeHumanReadable);
    CPPUNIT_TEST(testDeserializeDeserializeBinary);
    CPPUNIT_TEST_SUITE_END();
    
private:
    void testSerializeHumanReadable() {
        Test1 msg;
        msg.set_some_int(42);
        msg.set_some_text("Life, the universe and everything.");
        std::string buf;
        SerializerFactory::humanReadableSerializer().serialize(msg, buf);
        CPPUNIT_ASSERT_EQUAL(std::string("{") + "\n" +
            " \"someInt\": 42," + "\n" +
            " \"someText\": \"Life, the universe and everything.\"" + "\n" +
        "}\n", buf);
    }
    void testDeserializeHumanReadable() {
        std::string buf = std::string("{") + "\n" +
            " \"someInt\": 42," + "\n" +
            " \"someText\": \"Life, the universe and everything.\"" + "\n" +
        "}\n";
        Test1 msg;
        SerializerFactory::humanReadableSerializer().deserialize(msg, buf);
        CPPUNIT_ASSERT_EQUAL(42, msg.some_int());
        CPPUNIT_ASSERT_EQUAL(std::string("Life, the universe and everything."), msg.some_text());
    }
    
    void testSerializeDeserializeHumanReadable() {
        Test1 msg;
        msg.set_some_int(42);
        msg.set_some_text("Life, the universe and everything.");
        std::string buf;
        SerializerFactory::humanReadableSerializer().serialize(msg, buf);
        
        Test1 msg2;
        SerializerFactory::humanReadableSerializer().deserialize(msg2, buf);
        CPPUNIT_ASSERT_EQUAL(msg.some_int(), msg2.some_int());
        CPPUNIT_ASSERT_EQUAL(msg.some_text(), msg2.some_text());
    }
    
    void testDeserializeDeserializeBinary() {
        Test1 msg;
        msg.set_some_int(42);
        msg.set_some_text("Life, the universe and everything.");
        std::string buf;
        SerializerFactory::binarySerializer().serialize(msg, buf);
        
        Test1 msg2;
        SerializerFactory::binarySerializer().deserialize(msg2, buf);
        CPPUNIT_ASSERT_EQUAL(msg.some_int(), msg2.some_int());
        CPPUNIT_ASSERT_EQUAL(msg.some_text(), msg2.some_text());
    }
};

#endif /* SERIALIZABLETEST_H */

