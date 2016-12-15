#ifndef MACROS_H
#define MACROS_H

#define ABS(x) ((x) < 0 ? -1*(x) : (x))
#define FLOAT_EQUALS(f1, f2) (ABS((f1) - (f2)) < 1e-5)

#endif /* MACROS_H */

