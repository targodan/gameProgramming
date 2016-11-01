#include "Exception.h"

#include <cstdio>
#include <cstring>
#include <algorithm>

const char* defaultWhat = "Exception";

namespace engine {
    Exception::Exception() {}
    
    Exception::Exception(const char* what, ...) {
        va_list list;
        va_start(list, what);
        this->initFromList(what, list);
        va_end(list);
    }
    
    void Exception::initFromList(const char* what, va_list args) {
        this->buffSize = vsnprintf(nullptr, 0, what, args) + 1;
        this->_what = new char[this->buffSize];
        vsnprintf(this->_what, this->buffSize, what, args);
        this->_what[this->buffSize-1] = '\0';
    }
    
    Exception::Exception(const Exception& orig) {
        this->buffSize = orig.buffSize;
        this->_what = new char[this->buffSize];
        std::copy(orig._what, orig._what + orig.buffSize, this->_what);
    }

    Exception::~Exception() {
        if(this->_what != nullptr) {
            delete[] this->_what;
        }
    }

    const char* Exception::what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT {
        return this->_what;
    }
}

