#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <cstdarg>

#ifndef _GLIBCXX_TXN_SAFE_DYN
#define _GLIBCXX_TXN_SAFE_DYN
#endif
#ifndef _GLIBCXX_USE_NOEXCEPT
#define _GLIBCXX_USE_NOEXCEPT
#endif

namespace engine {
    class Exception : public std::exception {
    public:
        Exception(const char* what, ...);
        Exception(const Exception& orig);
        virtual ~Exception();

        virtual const char* what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT override;
    protected:
        Exception();
        void initFromList(const char* what, va_list args);
        
        int buffSize = 0;
        char* _what = nullptr;
    };
}

#define DECLARE_EXCEPTION(name) \
    class name : public Exception { \
    public: \
        name(const char* what, ...) { \
            va_list list; \
            va_start(list, what); \
            this->initFromList(what, list); \
            va_end(list); \
        } \
        name(const char* what, va_list list) { \
            this->initFromList(what, list); \
        } \
        name(const name& orig) : Exception(orig) {} \
        virtual ~name() {} \
    }; \

#endif /* EXCEPTION_H */

