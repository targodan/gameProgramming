#include "Exception.h"

#include <string.h>

namespace engine {
    Exception::Exception(const char* what) : _what(what) {
    }

    Exception::~Exception() {
    }

    const char* Exception::what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT {
        const char* div = " : ";
        const char* prev = std::exception::what();
        
        char* buff = new char[strlen(prev)+strlen(this->_what)+strlen(div)+1];
        buff[0] = '\0';
        
        strcat(buff, this->_what);
        strcat(buff, div);
        strcat(buff, prev);
        
        return buff;
    }
}

