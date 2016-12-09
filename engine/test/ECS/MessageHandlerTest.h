#ifndef MESSAGEHANDLERTEST_H
#define MESSAGEHANDLERTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <memory>
#include <random>
#include <algorithm>
#include <sstream>

#include "ECS/MessageHandler.h"
#include "CollisionException.h"
#include "IllegalArgumentException.h"
#include "EmptyQueueException.h"

#include "MockReceiver.h"

class MessageHandlerTest : public CPPUNIT_NS::TestFixture, engine::ECS::MessageHandler {
    CPPUNIT_TEST_SUITE(MessageHandlerTest);
    CPPUNIT_TEST(testRegisterMessage);
    CPPUNIT_TEST(testLoookupMessageId);
    CPPUNIT_TEST(testQueueMessage);
    CPPUNIT_TEST(testHasMessages);
    CPPUNIT_TEST(testPopMessage);
    CPPUNIT_TEST(testRegisterReceiver);
    CPPUNIT_TEST(testDispatchMessage);
    CPPUNIT_TEST(testUsualUsage);
    CPPUNIT_TEST_SUITE_END();
    
public:
    void setUp() override {
    }
    void tearDown() override {
        this->ids.clear();
        this->names.clear();
        this->nextId = 1;
        this->queue.clear();
        this->receivers.clear();
    }
    
private:
    void testRegisterMessage() {
        CPPUNIT_ASSERT_EQUAL(0ul, this->ids.size());
        CPPUNIT_ASSERT_EQUAL(0ul, this->names.size());
        
        this->registerMessage("test");
        CPPUNIT_ASSERT_EQUAL(1ul, this->ids.size());
        CPPUNIT_ASSERT_EQUAL(1ul, this->names.size());
        
        this->registerMessage("test2");
        CPPUNIT_ASSERT_EQUAL(2ul, this->ids.size());
        CPPUNIT_ASSERT_EQUAL(2ul, this->names.size());
        
        CPPUNIT_ASSERT_THROW_MESSAGE("Duplicate names should throw.",
                this->registerMessage("test"),
                engine::CollisionException);
        
        CPPUNIT_ASSERT_THROW_MESSAGE("Empty name not allowed.",
                this->registerMessage(""),
                engine::IllegalArgumentException);
    }
    
    void testLoookupMessageId() {
        auto id = this->registerMessage("test");
        CPPUNIT_ASSERT_EQUAL(id, this->lookupMessageId("test"));
        
        auto id2 = this->registerMessage("test2");
        CPPUNIT_ASSERT_EQUAL(id, this->lookupMessageId("test"));
        CPPUNIT_ASSERT_EQUAL(id2, this->lookupMessageId("test2"));
        
        CPPUNIT_ASSERT_THROW_MESSAGE("Looking up unregistered messages should throw.",
                this->lookupMessageId("unregistered"),
                engine::IllegalArgumentException);
    }
    
    void testQueueMessage() {
        CPPUNIT_ASSERT_EQUAL(0ul, this->queue.size());
        
        auto id = this->registerMessage("test");
        this->queueMessage(std::make_shared<engine::ECS::Message>(id));
        CPPUNIT_ASSERT_EQUAL(1ul, this->queue.size());
        
        this->queueMessage(std::make_shared<engine::ECS::Message>(id));
        CPPUNIT_ASSERT_EQUAL(2ul, this->queue.size());
        
        CPPUNIT_ASSERT_THROW_MESSAGE("Queueing unregistered message ids should throw.",
                this->queueMessage(std::make_shared<engine::ECS::Message>(42)),
                engine::IllegalArgumentException);
    }
    
    void testHasMessages() {
        CPPUNIT_ASSERT_EQUAL(false, this->hasMessages());
        
        auto id = this->registerMessage("test");
        this->queueMessage(std::make_shared<engine::ECS::Message>(id));
        CPPUNIT_ASSERT_EQUAL(true, this->hasMessages());
        
        this->popMessage();
        
        CPPUNIT_ASSERT_EQUAL(false, this->hasMessages());
    }
    
    void testPopMessage() {
        auto id = this->registerMessage("test");
        auto msg1 = std::make_shared<engine::ECS::Message>(id);
        this->queueMessage(msg1);
        auto msg2 = std::make_shared<engine::ECS::Message>(id);
        this->queueMessage(msg2);
        
        CPPUNIT_ASSERT_EQUAL(msg1, this->popMessage());
        CPPUNIT_ASSERT_EQUAL(msg2, this->popMessage());
        CPPUNIT_ASSERT_THROW_MESSAGE("Popping on an empty queue should throw.",
                this->popMessage(),
                engine::EmptyQueueException);
    }
    
    void testRegisterReceiver() {
        auto id = this->registerMessage("test");
        MockReceiver mr(nullptr);
        CPPUNIT_ASSERT_EQUAL(0ul, this->receivers[id].size());
        this->registerReceiver(id, &mr);
        CPPUNIT_ASSERT_EQUAL(1ul, this->receivers[id].size());
        
        auto id2 = this->registerMessage("test2");
        CPPUNIT_ASSERT_EQUAL(0ul, this->receivers[id2].size());
        this->registerReceiver(id2, &mr);
        CPPUNIT_ASSERT_EQUAL(1ul, this->receivers[id2].size());
        CPPUNIT_ASSERT_EQUAL(1ul, this->receivers[id].size());
    }
    
    void testDispatchMessage() {
        size_t counter1 = 0;
        size_t counter2 = 0;
        auto id = this->registerMessage("test");
        auto id2 = this->registerMessage("test2");
        MockReceiver mr(&counter1);
        MockReceiver mr2(&counter2);
        this->registerReceiver(id, &mr);
        this->registerReceiver(id2, &mr2);
        
        CPPUNIT_ASSERT_EQUAL(0ul, counter1);
        CPPUNIT_ASSERT_EQUAL(0ul, counter2);
        this->dispatch(std::make_shared<MockMessage>(id, 1));
        CPPUNIT_ASSERT_EQUAL(1ul, counter1);
        CPPUNIT_ASSERT_EQUAL(0ul, counter2);
        
        this->dispatch(std::make_shared<MockMessage>(id, 3));
        CPPUNIT_ASSERT_EQUAL(4ul, counter1);
        CPPUNIT_ASSERT_EQUAL(0ul, counter2);
        
        this->dispatch(std::make_shared<MockMessage>(id2, 2));
        CPPUNIT_ASSERT_EQUAL(4ul, counter1);
        CPPUNIT_ASSERT_EQUAL(2ul, counter2);
        
        this->dispatch(std::make_shared<MockMessage>(id2, 5));
        CPPUNIT_ASSERT_EQUAL(4ul, counter1);
        CPPUNIT_ASSERT_EQUAL(7ul, counter2);
        
        this->registerReceiver(id, &mr);
        
        this->dispatch(std::make_shared<MockMessage>(id, 5));
        CPPUNIT_ASSERT_EQUAL(14ul, counter1);
        CPPUNIT_ASSERT_EQUAL(7ul, counter2);
    }
    
    void testUsualUsage() {
        size_t counter = 0;
        auto id = this->registerMessage("test");
        MockReceiver mr(&counter);
        this->registerReceiver(id, &mr);
        
        this->dispatch(std::make_shared<MockMessage>(id, 1));
        this->dispatch(std::make_shared<MockMessage>(id, 1));
        this->dispatch(std::make_shared<MockMessage>(id, 1));
        this->dispatch(std::make_shared<MockMessage>(id, 1));
        this->dispatch(std::make_shared<MockMessage>(id, 1));
        
        while(this->hasMessages()) {
            auto msg = this->popMessage();
            this->dispatch(msg);
        }
        CPPUNIT_ASSERT_EQUAL(5ul, counter);
    }
};

#endif /* MESSAGEHANDLERTEST_H */

