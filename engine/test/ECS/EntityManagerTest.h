#ifndef ENTITYMANAGERTEST_H
#define ENTITYMANAGERTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <memory>
#include <random>
#include <algorithm>
#include <functional>
#include <sstream>

#include "ECS/EntityManager.h"
#include "ECS/Entity.h"

#include "IO/SerializerFactory.h"

#include "MockComponents.h"

#include "util/ostream_helper.h"
#include "../helper.h"

using engine::ECS::EntityManager;
using engine::ECS::Entity;
using engine::util::vector;

class EntityManagerTest : public CPPUNIT_NS::TestFixture, EntityManager {
    CPPUNIT_TEST_SUITE(EntityManagerTest);
    CPPUNIT_TEST(testCreateEntity);
    CPPUNIT_TEST(testAddComponent);
    CPPUNIT_TEST(testIterateComponents);
    CPPUNIT_TEST(testGetComponentOfEntity);
    CPPUNIT_TEST(testSort);
    CPPUNIT_TEST(testSerializeDeserializeHuman);
    CPPUNIT_TEST(testSerializeDeserializeBinary);
    CPPUNIT_TEST(testCreatePrefab);
    CPPUNIT_TEST_SUITE_END();
    
public:
    void setUp() override {
    }
    void tearDown() override {
        this->clear();
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
            CPPUNIT_ASSERT_EQUAL(1ul, this->m_components[Comp1::getComponentTypeId()].size());
            auto ptr = this->m_components[Comp1::getComponentTypeId()][0].get();
            CPPUNIT_ASSERT_EQUAL(1ul, dynamic_cast<Comp1*>(ptr)->getData());
        }
        {
            CPPUNIT_ASSERT_EQUAL(1ul, this->m_components[Comp2::getComponentTypeId()].size());
            auto ptr = this->m_components[Comp2::getComponentTypeId()][0].get();
            CPPUNIT_ASSERT_EQUAL(2ul, dynamic_cast<Comp2*>(ptr)->getData());
        }
        {
            CPPUNIT_ASSERT_EQUAL(1ul, this->m_components[Comp4::getComponentTypeId()].size());
            auto ptr = this->m_components[Comp4::getComponentTypeId()][0].get();
            CPPUNIT_ASSERT_EQUAL(4ul, dynamic_cast<Comp4*>(ptr)->getData());
        }
        {
            CPPUNIT_ASSERT_EQUAL(1ul, this->m_components[Comp5::getComponentTypeId()].size());
            auto ptr = this->m_components[Comp5::getComponentTypeId()][0].get();
            CPPUNIT_ASSERT_EQUAL(5ul, dynamic_cast<Comp5*>(ptr)->getData());
        }
        
        auto e2 = this->createEntity("Test2");
        e2.addComponent<Comp1>(6)
            .addComponent<Comp3>(7)
            .addComponent<Comp4>(8)
            .addComponent<Comp5>(9);
        
        {
            CPPUNIT_ASSERT_EQUAL(2ul, this->m_components[Comp1::getComponentTypeId()].size());
            auto ptr = this->m_components[Comp1::getComponentTypeId()][1].get();
            CPPUNIT_ASSERT_EQUAL(6ul, dynamic_cast<Comp1*>(ptr)->getData());
        }
        {
            CPPUNIT_ASSERT_EQUAL(1ul, this->m_components[Comp3::getComponentTypeId()].size());
            auto ptr = this->m_components[Comp3::getComponentTypeId()][0].get();
            CPPUNIT_ASSERT_EQUAL(7ul, dynamic_cast<Comp3*>(ptr)->getData());
        }
        {
            CPPUNIT_ASSERT_EQUAL(2ul, this->m_components[Comp4::getComponentTypeId()].size());
            auto ptr = this->m_components[Comp4::getComponentTypeId()][1].get();
            CPPUNIT_ASSERT_EQUAL(8ul, dynamic_cast<Comp4*>(ptr)->getData());
        }
        {
            CPPUNIT_ASSERT_EQUAL(2ul, this->m_components[Comp5::getComponentTypeId()].size());
            auto ptr = this->m_components[Comp5::getComponentTypeId()][1].get();
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
    
    void testGetComponentOfEntity() {
        auto e1 = this->createEntity("Test")
            .addComponent<Comp1>(1)
            .addComponent<Comp2>(2)
            .addComponent<Comp4>(3);
        auto e2 = this->createEntity("Test")
            .addComponent<Comp2>(4)
            .addComponent<Comp4>(5)
            .addComponent<Comp5>(6);
        
        CPPUNIT_ASSERT_EQUAL(1ul, 
                this->getComponentOfEntity(e1.getId(), Comp1::getComponentTypeId())->to<Comp1>().getData());
        CPPUNIT_ASSERT_EQUAL(2ul, 
                this->getComponentOfEntity(e1.getId(), Comp2::getComponentTypeId())->to<Comp2>().getData());
        CPPUNIT_ASSERT_EQUAL(3ul, 
                this->getComponentOfEntity(e1.getId(), Comp4::getComponentTypeId())->to<Comp4>().getData());
        CPPUNIT_ASSERT_EQUAL(4ul, 
                this->getComponentOfEntity(e2.getId(), Comp2::getComponentTypeId())->to<Comp2>().getData());
        CPPUNIT_ASSERT_EQUAL(5ul, 
                this->getComponentOfEntity(e2.getId(), Comp4::getComponentTypeId())->to<Comp4>().getData());
        CPPUNIT_ASSERT_EQUAL(6ul, 
                this->getComponentOfEntity(e2.getId(), Comp5::getComponentTypeId())->to<Comp5>().getData());
    }
    
    void testSort() {
        auto e1 = this->createEntity("Test")
            .addComponent<Comp1>(1)
            .addComponent<Comp2>(2)
            .addComponent<Comp4>(3)
            .addComponent<SortComponent>(2);
        auto e2 = this->createEntity("Test")
            .addComponent<Comp2>(4)
            .addComponent<Comp4>(5)
            .addComponent<Comp5>(6)
            .addComponent<SortComponent>(1);
        auto e3 = this->createEntity("Test")
            .addComponent<Comp2>(7)
            .addComponent<Comp4>(8)
            .addComponent<SortComponent>(3);
        auto e4 = this->createEntity("Test")
            .addComponent<Comp1>(9);
        auto e5 = this->createEntity("Test")
            .addComponent<Comp1>(10)
            .addComponent<Comp2>(11)
            .addComponent<Comp4>(12)
            .addComponent<SortComponent>(4);
        
        vector<size_t> comp1Order = {9, 1, 10};
        vector<size_t> comp2Order = {4, 2, 7, 11};
        vector<size_t> comp4Order = {5, 3, 8, 12};
        vector<size_t> comp5Order = {6};
        vector<size_t> sortCompOrder = {1, 2, 3, 4};
        
        this->sort(SortComponent::getComponentTypeId(), [](const auto& _l, const auto& _r) {
            auto& l = static_cast<SortComponent&>(*_l.get());
            auto& r = static_cast<SortComponent&>(*_r.get());
            return l.getOrder() < r.getOrder();
        });
        
        auto comp1 = map_to<std::shared_ptr<engine::ECS::Component>, size_t, vector<size_t>>(this->m_components[Comp1::getComponentTypeId()], [](const auto& _c) {
            const auto& c = static_cast<const Comp1&>(*_c.get());
            return c.getData();
        });
        auto comp2 = map_to<std::shared_ptr<engine::ECS::Component>, size_t, vector<size_t>>(this->m_components[Comp2::getComponentTypeId()], [](const auto& _c) {
            const auto& c = static_cast<const Comp2&>(*_c.get());
            return c.getData();
        });
        auto comp4 = map_to<std::shared_ptr<engine::ECS::Component>, size_t, vector<size_t>>(this->m_components[Comp4::getComponentTypeId()], [](const auto& _c) {
            const auto& c = static_cast<const Comp4&>(*_c.get());
            return c.getData();
        });
        auto comp5 = map_to<std::shared_ptr<engine::ECS::Component>, size_t, vector<size_t>>(this->m_components[Comp5::getComponentTypeId()], [](const auto& _c) {
            const auto& c = static_cast<const Comp5&>(*_c.get());
            return c.getData();
        });
        auto sortComp = map_to<std::shared_ptr<engine::ECS::Component>, size_t, vector<size_t>>(this->m_components[SortComponent::getComponentTypeId()], [](const auto& _c) {
            const auto& c = static_cast<const SortComponent&>(*_c.get());
            return c.getOrder();
        });
        
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Not sorted correctly!", comp1Order, comp1);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Not sorted correctly!", comp2Order, comp2);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Not sorted correctly!", comp4Order, comp4);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Not sorted correctly!", comp5Order, comp5);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Not sorted correctly!", sortCompOrder, sortComp);
        
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Sorting breaks getComponentOfEntity!", 1ul, 
                this->getComponentOfEntity(e1.getId(), Comp1::getComponentTypeId())->to<Comp1>().getData());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Sorting breaks getComponentOfEntity!", 2ul, 
                this->getComponentOfEntity(e1.getId(), Comp2::getComponentTypeId())->to<Comp2>().getData());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Sorting breaks getComponentOfEntity!", 3ul, 
                this->getComponentOfEntity(e1.getId(), Comp4::getComponentTypeId())->to<Comp4>().getData());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Sorting breaks getComponentOfEntity!", 2ul, 
                this->getComponentOfEntity(e1.getId(), SortComponent::getComponentTypeId())->to<SortComponent>().getOrder());
        
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Sorting breaks getComponentOfEntity!", 4ul, 
                this->getComponentOfEntity(e2.getId(), Comp2::getComponentTypeId())->to<Comp2>().getData());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Sorting breaks getComponentOfEntity!", 5ul, 
                this->getComponentOfEntity(e2.getId(), Comp4::getComponentTypeId())->to<Comp4>().getData());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Sorting breaks getComponentOfEntity!", 6ul, 
                this->getComponentOfEntity(e2.getId(), Comp5::getComponentTypeId())->to<Comp5>().getData());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Sorting breaks getComponentOfEntity!", 1ul, 
                this->getComponentOfEntity(e2.getId(), SortComponent::getComponentTypeId())->to<SortComponent>().getOrder());
        
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Sorting breaks getComponentOfEntity!", 7ul, 
                this->getComponentOfEntity(e3.getId(), Comp2::getComponentTypeId())->to<Comp2>().getData());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Sorting breaks getComponentOfEntity!", 8ul, 
                this->getComponentOfEntity(e3.getId(), Comp4::getComponentTypeId())->to<Comp4>().getData());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Sorting breaks getComponentOfEntity!", 3ul, 
                this->getComponentOfEntity(e3.getId(), SortComponent::getComponentTypeId())->to<SortComponent>().getOrder());
        
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Sorting breaks getComponentOfEntity!", 9ul, 
                this->getComponentOfEntity(e4.getId(), Comp1::getComponentTypeId())->to<Comp1>().getData());
        
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Sorting breaks getComponentOfEntity!", 10ul, 
                this->getComponentOfEntity(e5.getId(), Comp1::getComponentTypeId())->to<Comp1>().getData());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Sorting breaks getComponentOfEntity!", 11ul, 
                this->getComponentOfEntity(e5.getId(), Comp2::getComponentTypeId())->to<Comp2>().getData());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Sorting breaks getComponentOfEntity!", 12ul, 
                this->getComponentOfEntity(e5.getId(), Comp4::getComponentTypeId())->to<Comp4>().getData());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Sorting breaks getComponentOfEntity!", 4ul, 
                this->getComponentOfEntity(e5.getId(), SortComponent::getComponentTypeId())->to<SortComponent>().getOrder());
        
        // test iteration again
        {
            size_t i = 0;
            vector<size_t> exp2 = {4, 2, 7, 11};
            vector<size_t> exp4 = {5, 3, 8, 12};
            for(auto it = this->begin({Comp2::getComponentTypeId(), Comp4::getComponentTypeId()}); it != this->end(); ++it, ++i) {
                CPPUNIT_ASSERT_EQUAL_MESSAGE("Sorting breaks iterating.",
                        exp2[i], dynamic_cast<Comp2&>(*it[0]).getData());
                CPPUNIT_ASSERT_EQUAL_MESSAGE("Sorting breaks iterating.",
                        exp4[i], dynamic_cast<Comp4&>(*it[1]).getData());
            }
        }
    }
    
    engine::ECS::entityId_t serializedNextId;
    
    vector<Entity> fillForSerialization() {
        vector<Entity> ret;
        ret.push_back(this->createEntity("test1")
                .addComponent<Comp1>(42)
                .addComponent<Comp3>(666));
        ret.push_back(this->createEntity("test2")
                .addComponent<Comp1>(9)
                .addComponent<Comp2>(10));
        ret.push_back(this->createEntity("test3")
                .addComponent<Comp3>(90)
                .addComponent<Comp4>(100));
        
        serializedNextId = this->nextEntityId;
        return ret;
    }
    
    void testDeserialization(vector<Entity> entities) {
        CPPUNIT_ASSERT_EQUAL(serializedNextId, this->nextEntityId);
        
        CPPUNIT_ASSERT_EQUAL_MESSAGE("All entities must be present.", entities.size(), this->entityComponentIndexes.size());
        for(auto e : entities) {
            CPPUNIT_ASSERT_MESSAGE("All entities must be present.", this->entityComponentIndexes.find(e.getId()) != this->entityComponentIndexes.end());
        }
        
        CPPUNIT_ASSERT_EQUAL_MESSAGE("All serializable components must be present.", 6ul, this->serializables.size());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("All serializable components must be present.",
                42ul, this->serializables[0]->to<Comp1>().getData());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("All serializable components must be present.",
                666ul, this->serializables[1]->to<Comp3>().getData());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("All serializable components must be present.",
                9ul, this->serializables[2]->to<Comp1>().getData());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("All serializable components must be present.",
                10ul, this->serializables[3]->to<Comp2>().getData());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("All serializable components must be present.",
                90ul, this->serializables[4]->to<Comp3>().getData());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("All serializable components must be present.",
                100ul, this->serializables[5]->to<Comp4>().getData());
        
        CPPUNIT_ASSERT_EQUAL_MESSAGE("All components must be present.",
                42ul, this->getComponentOfEntity(entities[0].getId(), Comp1::getComponentTypeId())->to<Comp1>().getData());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("All components must be present.",
                666ul, this->getComponentOfEntity(entities[0].getId(), Comp3::getComponentTypeId())->to<Comp3>().getData());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("All components must be present.",
                9ul, this->getComponentOfEntity(entities[1].getId(), Comp1::getComponentTypeId())->to<Comp1>().getData());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("All components must be present.",
                10ul, this->getComponentOfEntity(entities[1].getId(), Comp2::getComponentTypeId())->to<Comp2>().getData());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("All components must be present.",
                90ul, this->getComponentOfEntity(entities[2].getId(), Comp3::getComponentTypeId())->to<Comp3>().getData());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("All components must be present.",
                100ul, this->getComponentOfEntity(entities[2].getId(), Comp4::getComponentTypeId())->to<Comp4>().getData());
    }
    
    void testSerializeDeserializeHuman() {
        auto entities = fillForSerialization();
        
        std::string buf;
        engine::IO::SerializerFactory::humanReadableSerializer().serialize(*this, buf);
        
        this->clear();
        engine::IO::SerializerFactory::humanReadableSerializer().deserialize(*this, buf);
        
        testDeserialization(entities);
    }
    
    void testSerializeDeserializeBinary() {
        auto entities = fillForSerialization();
        
        std::string buf;
        engine::IO::SerializerFactory::binarySerializer().serialize(*this, buf);
        
        this->clear();
        engine::IO::SerializerFactory::binarySerializer().deserialize(*this, buf);
        
        testDeserialization(entities);
    }
    
    void testCreatePrefab() {
        auto& serializer = engine::IO::SerializerFactory::humanReadableSerializer();
        auto e1 = this->createEntityFromPrefab(serializer,
                "{"
                "   \"entityName\": \"test1\","
                "   \"components\": ["
                "       {"
                "           \"componentTypeName\": \"Comp1\","
                "           \"component\": {"
                "               \"@type\": \"type.googleapis.com/pb.Comp1\","
                "               \"data\": 42"
                "           }"
                "       }"
                "   ]"
                "}"
            );
        
        CPPUNIT_ASSERT_EQUAL(std::string("test1"), e1.getName());
        CPPUNIT_ASSERT_EQUAL(1ul, this->m_components.size());
        CPPUNIT_ASSERT_EQUAL(1ul, this->m_components[Comp1::getComponentTypeId()].size());
        CPPUNIT_ASSERT_EQUAL(42ul, this->getComponentOfEntity(e1.getId(), Comp1::getComponentTypeId())->to<Comp1>().getData());
        
        auto e2 = this->createEntityFromPrefab(serializer,
                "{"
                "   \"entityName\": \"test2\","
                "   \"components\": ["
                "       {"
                "           \"componentTypeName\": \"Comp3\","
                "           \"component\": {"
                "               \"@type\": \"type.googleapis.com/pb.Comp3\","
                "               \"data\": 666"
                "           }"
                "       },"
                "       {"
                "           \"componentTypeName\": \"Comp5\","
                "           \"component\": {"
                "               \"@type\": \"type.googleapis.com/pb.Comp5\","
                "               \"data\": 12"
                "           }"
                "       }"
                "   ]"
                "}"
            );
        
        CPPUNIT_ASSERT_EQUAL(std::string("test2"), e2.getName());
        CPPUNIT_ASSERT_EQUAL(3ul, this->m_components.size());
        CPPUNIT_ASSERT_EQUAL(1ul, this->m_components[Comp1::getComponentTypeId()].size());
        CPPUNIT_ASSERT_EQUAL(1ul, this->m_components[Comp3::getComponentTypeId()].size());
        CPPUNIT_ASSERT_EQUAL(1ul, this->m_components[Comp5::getComponentTypeId()].size());
        CPPUNIT_ASSERT_EQUAL(666ul, this->getComponentOfEntity(e2.getId(), Comp3::getComponentTypeId())->to<Comp3>().getData());
        CPPUNIT_ASSERT_EQUAL(12ul, this->getComponentOfEntity(e2.getId(), Comp5::getComponentTypeId())->to<Comp5>().getData());
    }
};

#endif /* ENTITYMANAGERTEST_H */

