#ifndef MACROS_H
#define MACROS_H

#define ECS_CONCAT(s1, s2) s1 ## s2
#define ECS_MAKE_UNIQUE_NAME(name) ECS_CONCAT(name, __LINE__)

#endif /* MACROS_H */

