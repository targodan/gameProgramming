#ifndef ENTITYMANAGERTEST_H
#define ENTITYMANAGERTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <memory>
#include <random>
#include <algorithm>
#include <sstream>

#include "ECS/EntityManager.h"
#include "ECS/Entity.h"

#include "MockComponents.h"

using engine::ECS::EntityManager;
using engine::ECS::Entity;

class EntityManagerTest : public CPPUNIT_NS::TestFixture, EntityManager {
    CPPUNIT_TEST_SUITE(EntityManagerTest);
    CPPUNIT_TEST(testCreateEntity);
    CPPUNIT_TEST(testAddComponent);
    CPPUNIT_TEST(testIterateComponents);
    CPPUNIT_TEST_SUITE_END();
    
public:
    void setUp() override {
    }
    void tearDown() override {
        this->nextEntityId = 0;
        this->components.clear();
    }
    
private:
    void testCreateEntity() {
        const size_t NUM_ENTITIES = 100;
        vector<Entity> entities;
        entities.reserve(NUM_ENTITIES);
        
        for(size_t i = 0; i < NUM_ENTITIES; ++i) {
            std::stringstream ss;
            ss << "TestEntity" << i;
            entities.push_back(this->createEntity(ss.str()));
        }
        
        for(size_t i = 0; i < NUM_ENTITIES; ++i) {
            auto id = entities[i].getId();
            for(size_t j = i+1; j < NUM_ENTITIES; ++j) {
                CPPUNIT_ASSERT_MESSAGE("IDs should be unique!",
                        entities[j].getId() != id);
            }
        }
    }
    
    void testAddComponent() {
        auto e = this->createEntity("Test");
        e.addComponent<Comp1>(1)
            .addComponent<Comp2>(2)
            .addComponent<Comp4>(4)
            .addComponent<Comp5>(5);
        
        {
            CPPUNIT_ASSERT_EQUAL(1ul, this->components[Comp1::getComponentTypeId()].size());
            auto ptr = this->components[Comp1::getComponentTypeId()][0].get();
            CPPUNIT_ASSERT_EQUAL(1ul, dynamic_cast<Comp1*>(ptr)->getData());
        }
        {
            CPPUNIT_ASSERT_EQUAL(1ul, this->components[Comp2::getComponentTypeId()].size());
            auto ptr = this->components[Comp2::getComponentTypeId()][0].get();
            CPPUNIT_ASSERT_EQUAL(2ul, dynamic_cast<Comp2*>(ptr)->getData());
        }
        {
            CPPUNIT_ASSERT_EQUAL(1ul, this->components[Comp4::getComponentTypeId()].size());
            auto ptr = this->components[Comp4::getComponentTypeId()][0].get();
            CPPUNIT_ASSERT_EQUAL(4ul, dynamic_cast<Comp4*>(ptr)->getData());
        }
        {
            CPPUNIT_ASSERT_EQUAL(1ul, this->components[Comp5::getComponentTypeId()].size());
            auto ptr = this->components[Comp5::getComponentTypeId()][0].get();
            CPPUNIT_ASSERT_EQUAL(5ul, dynamic_cast<Comp5*>(ptr)->getData());
        }
        
        auto e2 = this->createEntity("Test2");
        e2.addComponent<Comp1>(6)
            .addComponent<Comp3>(7)
            .addComponent<Comp4>(8)
            .addComponent<Comp5>(9);
        
        {
            CPPUNIT_ASSERT_EQUAL(2ul, this->components[Comp1::getComponentTypeId()].size());
            auto ptr = this->components[Comp1::getComponentTypeId()][1].get();
            CPPUNIT_ASSERT_EQUAL(6ul, dynamic_cast<Comp1*>(ptr)->getData());
        }
        {
            CPPUNIT_ASSERT_EQUAL(1ul, this->components[Comp3::getComponentTypeId()].size());
            auto ptr = this->components[Comp3::getComponentTypeId()][0].get();
            CPPUNIT_ASSERT_EQUAL(7ul, dynamic_cast<Comp3*>(ptr)->getData());
        }
        {
            CPPUNIT_ASSERT_EQUAL(2ul, this->components[Comp4::getComponentTypeId()].size());
            auto ptr = this->components[Comp4::getComponentTypeId()][1].get();
            CPPUNIT_ASSERT_EQUAL(8ul, dynamic_cast<Comp4*>(ptr)->getData());
        }
        {
            CPPUNIT_ASSERT_EQUAL(2ul, this->components[Comp5::getComponentTypeId()].size());
            auto ptr = this->components[Comp5::getComponentTypeId()][1].get();
            CPPUNIT_ASSERT_EQUAL(9ul, dynamic_cast<Comp5*>(ptr)->getData());
        }
    }
    
    void testIterateComponents() {
        {
            auto e = this->createEntity("Test");
            e.addComponent<Comp1>(1)
                .addComponent<Comp2>(2)
                .addComponent<Comp4>(3);
        }
        {
            auto e = this->createEntity("Test");
            e.addComponent<Comp2>(4)
                .addComponent<Comp4>(5)
                .addComponent<Comp5>(6);
        }
        {
            auto e = this->createEntity("Test");
            e.addComponent<Comp2>(7)
                .addComponent<Comp4>(8);
        }
        {
            auto e = this->createEntity("Test");
            e.addComponent<Comp1>(9);
        }
        {
            auto e = this->createEntity("Test");
            e.addComponent<Comp1>(10)
                .addComponent<Comp2>(11)
                .addComponent<Comp4>(12);
        }
        {
            size_t i = 0;
            vector<size_t> exp = {1, 9, 10};
            for(auto it = this->begin({Comp1::getComponentTypeId()}); it != this->end(); ++it) {
                auto& comp = dynamic_cast<Comp1&>(**it);
                CPPUNIT_ASSERT_EQUAL_MESSAGE("Something went wrong while iterating.",
                        exp[i++], comp.getData());
            }
        }
        {
            size_t i = 0;
            vector<size_t> exp = {3, 5, 8, 12};
            for(auto it = this->begin({Comp4::getComponentTypeId()}); it != this->end(); ++it) {
                auto& comp = dynamic_cast<Comp4&>(**it);
                CPPUNIT_ASSERT_EQUAL_MESSAGE("Something went wrong while iterating.",
                        exp[i++], comp.getData());
            }
        }
        {
            size_t i = 0;
            vector<size_t> exp1 = {1, 10};
            vector<size_t> exp2 = {2, 11};
            for(auto it = this->begin({Comp1::getComponentTypeId(), Comp2::getComponentTypeId()}); it != this->end(); ++it, ++i) {
                CPPUNIT_ASSERT_EQUAL_MESSAGE("Something went wrong while iterating.",
                        exp1[i], dynamic_cast<Comp1&>(*it[0]).getData());
                CPPUNIT_ASSERT_EQUAL_MESSAGE("Something went wrong while iterating.",
                        exp2[i], dynamic_cast<Comp2&>(*it[1]).getData());
            }
        }
        {
            size_t i = 0;
            vector<size_t> exp2 = {2, 4, 7, 11};
            vector<size_t> exp4 = {3, 5, 8, 12};
            for(auto it = this->begin({Comp2::getComponentTypeId(), Comp4::getComponentTypeId()}); it != this->end(); ++it, ++i) {
                CPPUNIT_ASSERT_EQUAL_MESSAGE("Something went wrong while iterating.",
                        exp2[i], dynamic_cast<Comp2&>(*it[0]).getData());
                CPPUNIT_ASSERT_EQUAL_MESSAGE("Something went wrong while iterating.",
                        exp4[i], dynamic_cast<Comp4&>(*it[1]).getData());
            }
        }
        {
            size_t i = 0;
            vector<size_t> exp1 = {1, 10};
            vector<size_t> exp2 = {2, 11};
            vector<size_t> exp4 = {3, 12};
            for(auto it = this->begin({Comp1::getComponentTypeId(), Comp2::getComponentTypeId(), Comp4::getComponentTypeId()}); it != this->end(); ++it, ++i) {
                CPPUNIT_ASSERT_EQUAL_MESSAGE("Something went wrong while iterating.",
                        exp1[i], dynamic_cast<Comp1&>(*it[0]).getData());
                CPPUNIT_ASSERT_EQUAL_MESSAGE("Something went wrong while iterating.",
                        exp2[i], dynamic_cast<Comp2&>(*it[1]).getData());
                CPPUNIT_ASSERT_EQUAL_MESSAGE("Something went wrong while iterating.",
                        exp4[i], dynamic_cast<Comp4&>(*it[2]).getData());
            }
        }
    }
};

#endif /* ENTITYMANAGERTEST_H */

