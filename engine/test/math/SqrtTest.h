#ifndef SQRTTEST_H
#define SQRTTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include <math.h>
#include <chrono>

#include "../macros.h"

#include "math/functions.h"

class SqrtTest : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(SqrtTest);
    CPPUNIT_TEST(testPrecision_singleIt);
    CPPUNIT_TEST(testPrecision_doubleIt);
    CPPUNIT_TEST(benchPrecision_singleIt);
    CPPUNIT_TEST(benchPrecision_doubleIt);
    CPPUNIT_TEST_SUITE_END();

private:
    void testPrecision_singleIt() {
        double maxErr = 0;
        double errSum = 0;
        size_t i = 0;
        for(float f = 0; f < 100.f; f += 0.001) {
            double res = 1/engine::math::fastInverseSqrt_SingleIt(f);
            double exp = ::sqrt(f);
            double err = ABS(res - exp);
            errSum += err;
            if(maxErr < err) {
                maxErr = err;
            }
            ++i;
        }
        double avgErr = errSum / i;
        std::cout << " AvgErr = " << avgErr << " MaxErr = " << maxErr << " ";
    }
    void testPrecision_doubleIt() {
        double maxErr = 0;
        double errSum = 0;
        size_t i = 0;
        for(float f = 0; f < 100.f; f += 0.001) {
            double res = 1/engine::math::fastInverseSqrt_DoubleIt(f);
            double exp = ::sqrt(f);
            double err = ABS(res - exp);
            errSum += err;
            if(maxErr < err) {
                maxErr = err;
            }
            ++i;
        }
        double avgErr = errSum / i;
        std::cout << " AvgErr = " << avgErr << " MaxErr = " << maxErr << " ";
    }
    
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
    void benchPrecision_singleIt() {
        const size_t NUM_CALLS = 100000;
        size_t numValues = 0;
        
        double totalSpeedup = 0;
        double maxSpeedup = 0;
        double minSpeedup = 1e100;
        
        for(float f = 0; f < 100.f; f += 1) {
            auto start = std::chrono::high_resolution_clock::now();
            for(size_t i = 0; i < NUM_CALLS; ++i) {
                volatile float res = engine::math::fastInverseSqrt_SingleIt(f);
            }
            auto end = std::chrono::high_resolution_clock::now();
            auto diff = end - start;
            auto timeFISR = std::chrono::duration<unsigned long long, std::nano>(diff).count() / NUM_CALLS;
            
            start = std::chrono::high_resolution_clock::now();
            for(size_t i = 0; i < NUM_CALLS; ++i) {
                volatile float res = ::sqrt(f);
            }
            end = std::chrono::high_resolution_clock::now();
            diff = end - start;
            auto time = std::chrono::duration<unsigned long long, std::nano>(diff).count() / NUM_CALLS;
            
            double speedup = static_cast<double>(time) / static_cast<double>(timeFISR);
            totalSpeedup += speedup;
            if(maxSpeedup < speedup) {
                maxSpeedup = speedup;
            }
            if(minSpeedup > speedup) {
                minSpeedup = speedup;
            }
            ++numValues;
        }
        double avgSpeedup = totalSpeedup / numValues;
        
        std::cout << " MinSpeedup = " << minSpeedup * 100 << "% AvgSpeedup = "
                << avgSpeedup * 100 << "% MaxSpeedup = " << maxSpeedup * 100 << "% ";
    }
    
    void benchPrecision_doubleIt() {
        const size_t NUM_CALLS = 100000;
        size_t numValues = 0;
        
        double totalSpeedup = 0;
        double maxSpeedup = 0;
        double minSpeedup = 1e100;
        
        for(float f = 0; f < 100.f; f += 1) {
            auto start = std::chrono::high_resolution_clock::now();
            for(size_t i = 0; i < NUM_CALLS; ++i) {
                volatile float res = engine::math::fastInverseSqrt_DoubleIt(f);
            }
            auto end = std::chrono::high_resolution_clock::now();
            auto diff = end - start;
            auto timeFISR = std::chrono::duration<unsigned long long, std::nano>(diff).count() / NUM_CALLS;
            
            start = std::chrono::high_resolution_clock::now();
            for(size_t i = 0; i < NUM_CALLS; ++i) {
                volatile float res = ::sqrt(f);
            }
            end = std::chrono::high_resolution_clock::now();
            diff = end - start;
            auto time = std::chrono::duration<unsigned long long, std::nano>(diff).count() / NUM_CALLS;
            
            double speedup = static_cast<double>(time) / static_cast<double>(timeFISR);
            totalSpeedup += speedup;
            if(maxSpeedup < speedup) {
                maxSpeedup = speedup;
            }
            if(minSpeedup > speedup) {
                minSpeedup = speedup;
            }
            ++numValues;
        }
        double avgSpeedup = totalSpeedup / numValues;
        
        std::cout << " MinSpeedup = " << minSpeedup * 100 << "% AvgSpeedup = "
                << avgSpeedup * 100 << "% MaxSpeedup = " << maxSpeedup * 100 << "% ";
    }
#pragma GCC diagnostic pop
};


#endif /* SQRTTEST_H */

