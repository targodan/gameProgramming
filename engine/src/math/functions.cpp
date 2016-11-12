#include "functions.h"

#include <math.h>
#include <cstring>
#include <stdlib.h>

namespace engine {
    namespace math {
        float fastInverseSqrt_SingleIt(float number) {
            int32_t i;
            float x2, y;
            const float threehalfs = 1.5f;

            x2 = number * 0.5f;
            y  = number;
            std::memcpy(&i, &y, sizeof(int32_t));
            i  = 0x5f3759df - (i >> 1);               // what the fuck? 
            std::memcpy(&y, &i, sizeof(float));
            y  = y * (threehalfs - (x2 * y * y));     // 1st iteration

            return y;
        }
        
        float fastInverseSqrt_DoubleIt(float number) {
            int32_t i;
            float x2, y;
            const float threehalfs = 1.5f;

            x2 = number * 0.5f;
            y  = number;
            std::memcpy(&i, &y, sizeof(int32_t));
            i  = 0x5f3759df - (i >> 1);               // what the fuck? 
            std::memcpy(&y, &i, sizeof(float));
            y  = y * (threehalfs - (x2 * y * y));     // 1st iteration
            y  = y * (threehalfs - (x2 * y * y));     // 2nd iteration, this can be removed

            return y;
        }
        
        float fastInverseSqrt(float number) {
            return fastInverseSqrt_DoubleIt(number);
        }

        float sqrt(float f) {
        #ifdef USE_FAST_INVERSE_SQRT
            return 1. / fastInverseSqrt(f);
        #else
            return ::sqrt(f);
        #endif
        }
    }
}