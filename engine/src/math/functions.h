#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define IS_FLOAT_ZERO(x) (::abs(x) < 1e-5)

namespace engine {
    namespace math {
        float fastInverseSqrt(float number);
        float fastInverseSqrt_SingleIt(float number);
        float fastInverseSqrt_DoubleIt(float number);
        float sqrt(float f);
    }
}

#endif /* FUNCTIONS_H */

