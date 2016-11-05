/* 
 * File:   MapTest.cpp
 * Author: Luca Corbatto<corbatto@ika.rwth-aachen.de>
 * 
 * Created on 3. August 2015, 09:39
 */

#include "MapTest.h"
#include <string>
#include <stdlib.h>
#include <time.h>

CPPUNIT_TEST_SUITE_REGISTRATION(MapTest); 

MapTest::MapTest() {
}

MapTest::~MapTest() {
}

void gen_random(char *s, const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "_-:";

    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] = 0;
}

void MapTest::setUp() {
    for(rep_loop i = 0; i < BENCHMARKING_REPETITION; ++i) {
        int len = rand() % 20 + 20;
        char* tmp = new char[len+1];
        gen_random(tmp, len);
        m_strings[i] = std::string(tmp);
    }
}

void MapTest::tearDown() {
}

void MapTest::benchOverhead() {
    timespec ts, ts2;
    clock_gettime(CLOCK_REALTIME, &ts);
    for(rep_loop i = 0; i < BENCHMARKING_REPETITION; ++i) {
        // here will be the method to measure
    }
    clock_gettime(CLOCK_REALTIME, &ts2);
    
    uint64_t nano = ts2.tv_nsec - ts.tv_nsec;
    
    std::cout << "\t\t" << (nano / (double)BENCHMARKING_REPETITION) << " ns";
    
    CPPUNIT_ASSERT(true);
}

void MapTest::benchXorHash() {
    __XorHash<std::string> hasher;
    
    timespec ts, ts2;
    clock_gettime(CLOCK_REALTIME, &ts);
    for(rep_loop i = 0; i < BENCHMARKING_REPETITION; ++i) {
        hasher(m_strings[i]);
    }
    clock_gettime(CLOCK_REALTIME, &ts2);
    
    uint64_t nano = ts2.tv_nsec - ts.tv_nsec;
    
    std::cout << "\t\t" << (nano / (double)BENCHMARKING_REPETITION) << " ns";
    
    CPPUNIT_ASSERT(true);
}

void MapTest::benchFNVHash() {
    __FNVHash<std::string, sizeof(size_t)> hasher;
    
    timespec ts, ts2;
    clock_gettime(CLOCK_REALTIME, &ts);
    for(rep_loop i = 0; i < BENCHMARKING_REPETITION; ++i) {
        hasher(m_strings[i]);
    }
    clock_gettime(CLOCK_REALTIME, &ts2);
    
    uint64_t nano = ts2.tv_nsec - ts.tv_nsec;
    
    std::cout << "\t\t" << (nano / (double)BENCHMARKING_REPETITION) << " ns";
    
    CPPUNIT_ASSERT(true);
}

void MapTest::benchPrimeHash() {
    __PrimeHash<std::string> hasher;
    
    timespec ts, ts2;
    clock_gettime(CLOCK_REALTIME, &ts);
    for(rep_loop i = 0; i < BENCHMARKING_REPETITION; ++i) {
        hasher(m_strings[i]);
    }
    clock_gettime(CLOCK_REALTIME, &ts2);
    
    uint64_t nano = ts2.tv_nsec - ts.tv_nsec;
    
    std::cout << "\t\t" << (nano / (double)BENCHMARKING_REPETITION) << " ns";
    
    CPPUNIT_ASSERT(true);
}