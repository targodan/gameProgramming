#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>

namespace engine {
    class Exception : public std::exception {
    public:
        Exception(const char* what);
        virtual ~Exception();

        virtual const char* what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT override;
    private:
        const char* _what;
    };
}

#endif /* EXCEPTION_H */

