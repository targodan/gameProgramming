#ifndef THREAD_HELPER_H
#define THREAD_HELPER_H

#include <cstdarg>
#include <cstdlib>

#ifdef __unix__
// Support for naming threads on UNIX systems.
#include <pthread.h>
#endif

namespace engine {
    namespace util {
        inline void nameThisThread(const char* format, ...) {
            #ifdef __unix__
            va_list args;
            va_start(args, format);
            va_list tmpArgs;
            va_copy(tmpArgs, args);
            
            size_t buffSize = vsnprintf(nullptr, 0, format, tmpArgs) + 1;
            char* buff = new char[buffSize];
            vsnprintf(buff, buffSize, format, args);
            buff[buffSize-1] = '\0';
            
            va_end(tmpArgs);
            va_end(args);
            
            // Support for naming threads on POSIX systems.
            pthread_setname_np(pthread_self(), buff);
            
            delete[] buff;
            #endif
        }
    }
}
                    
#endif /* THREAD_HELPER_H */

