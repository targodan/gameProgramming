#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <cstdarg>

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

#endif /* EXCEPTION_H */

