#include "functions.h"

#include <math.h>
#include <cstring>

namespace engine {
    namespace math {
        float fastInverseSqrt(float number) {
            long i;
            float x2, y;
            const float threehalfs = 1.5f;

            x2 = number * 0.5f;
            y  = number;
//            i  = *reinterpret_cast<long*>(&y);        // evil floating point bit level hacking
            std::memcpy(&i, &y, sizeof(float));
            i  = 0x5f3759df - (i >> 1);               // what the fuck? 
//            y  = *reinterpret_cast<float*>(&i);
            std::memcpy(&y, &i, sizeof(long));
            y  = y * (threehalfs - (x2 * y * y));     // 1st iteration
            y  = y * (threehalfs - (x2 * y * y));     // 2nd iteration, this can be removed

            return y;
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