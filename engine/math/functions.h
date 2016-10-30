#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define IS_FLOAT_ZERO(x) (::abs(x) < 1e-5)

namespace engine {
    namespace math {
        float sqrt(float f);
    }
}

#endif /* FUNCTIONS_H */

