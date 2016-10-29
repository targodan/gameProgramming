/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   exception.cpp
 * Author: corbatto
 * 
 * Created on 29 October 2016, 14:52
 */

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

