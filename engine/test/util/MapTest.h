/* 
 * File:   MapTest.h
 * Author: Luca Corbatto<corbatto@ika.rwth-aachen.de>
 *
 * Created on 3. August 2015, 09:39
 */

#ifndef MAPTEST_H
#define	MAPTEST_H

#include "util/Map.h"
#include "../bench.h"

using namespace engine::util;

#include <cppunit/extensions/HelperMacros.h>

class MapTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(MapTest);

    CPPUNIT_TEST(benchOverhead);
    
    CPPUNIT_TEST(benchXorHash);
    CPPUNIT_TEST(benchPrimeHash);
    CPPUNIT_TEST(benchFNVHash);

    CPPUNIT_TEST_SUITE_END();

public:
    MapTest();
    virtual ~MapTest();
    
    void setUp();
    void tearDown();
    
private:
    void benchOverhead();

    void benchXorHash();
    void benchPrimeHash();
    void benchFNVHash();
    
    std::string m_strings[BENCHMARKING_REPETITION];
};

#endif	/* MAPTEST_H */

