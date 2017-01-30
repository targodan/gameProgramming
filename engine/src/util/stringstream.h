#ifndef STRINGSTREAM_H
#define STRINGSTREAM_H

#include <mutex>
#include <sstream>
#include <utility>
#include <iostream>

namespace engine {
    namespace util {
        using stringstream = std::stringstream;
        
        template<typename char_t>
        class basic_stringstreamTS : public std::basic_stringstream<char_t> {
        protected:
            std::mutex mutex;
            
        public:
            explicit basic_stringstreamTS(std::ios_base::openmode __m = std::ios_base::out | std::ios_base::in)
            : std::basic_stringstream<char_t>(__m) {}
            explicit basic_stringstreamTS(const typename std::basic_stringstream<char_t>::__string_type& __str,
                               std::ios_base::openmode __m = std::ios_base::out | std::ios_base::in)
            : std::basic_stringstream<char_t>(__str, __m) {}
            virtual ~basic_stringstreamTS() {}

            basic_stringstreamTS(const basic_stringstreamTS&) = delete;
            basic_stringstreamTS(basic_stringstreamTS&& __rhs)
            : std::basic_stringstream<char_t>(std::move(__rhs)), mutex(std::move(__rhs.mutex)) {}

            basic_stringstreamTS& operator=(const basic_stringstreamTS&) = delete;

            basic_stringstreamTS& operator=(basic_stringstreamTS&& __rhs) {
                std::basic_stringstream<char_t>::operator=(std::move(__rhs));
                this->mutex = std::move(__rhs.mutex);
                return *this;
            }

            void swap(basic_stringstreamTS& __rhs) {
                std::basic_stringstream<char_t>::swap(__rhs);
                this->mutex = std::move(__rhs.mutex);
            }
            
            #define GENERATE_STRINSTREAM_IN_OPERATOR(type) basic_stringstreamTS& operator<<(const type& t) { \
                std::unique_lock<std::mutex> lock(this->mutex); \
                std::basic_stringstream<char_t>::operator<<(t); \
                return *this; \
            }
            
            #define GENERATE_STRINSTREAM_OUT_OPERATOR(type) basic_stringstreamTS& operator>>(type& t) { \
                std::unique_lock<std::mutex> lock(this->mutex); \
                std::basic_stringstream<char_t>::operator>>(t); \
                return *this; \
            }

            #define GENERATE_STRINSTREAM_IN_OUT_OPERATOR(type) GENERATE_STRINSTREAM_IN_OPERATOR(type) \
            GENERATE_STRINSTREAM_OUT_OPERATOR(type)

            GENERATE_STRINSTREAM_IN_OUT_OPERATOR(long)
            GENERATE_STRINSTREAM_IN_OUT_OPERATOR(unsigned long)
            GENERATE_STRINSTREAM_IN_OUT_OPERATOR(short)
            GENERATE_STRINSTREAM_IN_OUT_OPERATOR(unsigned short)
            GENERATE_STRINSTREAM_IN_OUT_OPERATOR(int)
            GENERATE_STRINSTREAM_IN_OUT_OPERATOR(unsigned int)
            GENERATE_STRINSTREAM_IN_OUT_OPERATOR(long long)
            GENERATE_STRINSTREAM_IN_OUT_OPERATOR(unsigned long long)
            GENERATE_STRINSTREAM_IN_OUT_OPERATOR(double)
            GENERATE_STRINSTREAM_IN_OUT_OPERATOR(float)
            GENERATE_STRINSTREAM_IN_OUT_OPERATOR(long double)
            GENERATE_STRINSTREAM_IN_OUT_OPERATOR(void*)
            GENERATE_STRINSTREAM_IN_OPERATOR(char_t*)
            
            basic_stringstreamTS& operator<<(const char_t& t) {
                std::unique_lock<std::mutex> lock(this->mutex);
                std::basic_stringstream<char_t>::write(&t, 1);
                return *this;
            }
            
            basic_stringstreamTS& operator>>(char_t& t) {
                std::unique_lock<std::mutex> lock(this->mutex);
                std::basic_stringstream<char_t>::get(t);
                return *this;
            }
            
            #undef GENERATE_STRINSTREAM_IN_OUT_OPERATOR
            #undef GENERATE_STRINSTREAM_IN_OPERATOR
            #undef GENERATE_STRINSTREAM_OUT_OPERATOR
            
            std::basic_string<char_t> str() {
                std::unique_lock<std::mutex> lock(this->mutex);
                return std::basic_stringstream<char_t>::str();
            }
            
            std::basic_string<char_t> toStringThenEmpty() {
                std::unique_lock<std::mutex> lock(this->mutex);
                std::basic_string<char_t> ret = std::basic_stringstream<char_t>::str();
                std::basic_stringstream<char_t>::str("");
                return ret;
            }
            
            void str(const std::basic_string<char_t>& s) {
                std::unique_lock<std::mutex> lock(this->mutex);
                return std::basic_stringstream<char_t>::str(s);
            }
            
            typename std::basic_stringstream<char_t>::__istream_type& flush() {
                std::unique_lock<std::mutex> lock(this->mutex);
                return std::basic_stringstream<char_t>::flush();
            }
            
            typename std::basic_stringstream<char_t>::int_type get() {
                std::unique_lock<std::mutex> lock(this->mutex);
                return std::basic_stringstream<char_t>::get();
            }
            
            typename std::basic_stringstream<char_t>::__istream_type& get(typename std::basic_stringstream<char_t>::char_type& c) {
                std::unique_lock<std::mutex> lock(this->mutex);
                return std::basic_stringstream<char_t>::get(c);
            }
            
            typename std::basic_stringstream<char_t>::__istream_type& get(typename std::basic_stringstream<char_t>::char_type* c, size_t s, typename std::basic_stringstream<char_t>::char_type delim) {
                std::unique_lock<std::mutex> lock(this->mutex);
                return std::basic_stringstream<char_t>::get(c, s, delim);
            }
            
            typename std::basic_stringstream<char_t>::__istream_type& get(typename std::basic_stringstream<char_t>::char_type* c, size_t s) {
                std::unique_lock<std::mutex> lock(this->mutex);
                return std::basic_stringstream<char_t>::get(c, s);
            }
            
            typename std::basic_stringstream<char_t>::__istream_type& getline(typename std::basic_stringstream<char_t>::char_type* c, size_t s, typename std::basic_stringstream<char_t>::char_type delim) {
                std::unique_lock<std::mutex> lock(this->mutex);
                return std::basic_stringstream<char_t>::get(c, s, delim);
            }
            
            typename std::basic_stringstream<char_t>::__istream_type& getline(typename std::basic_stringstream<char_t>::char_type* c, size_t s) {
                std::unique_lock<std::mutex> lock(this->mutex);
                return std::basic_stringstream<char_t>::get(c, s);
            }
        };
        
        using stringstreamTS = basic_stringstreamTS<char>;
        
        template<typename char_t>
        class basic_stringstreamInsertable : public std::basic_stringstream<char_t> {
        protected:
            bool insertMode = false;
            
        public:
            explicit basic_stringstreamInsertable(std::ios_base::openmode __m = std::ios_base::out | std::ios_base::in)
            : std::basic_stringstream<char_t>(__m) {}
            explicit basic_stringstreamInsertable(const typename std::basic_stringstream<char_t>::__string_type& __str,
                               std::ios_base::openmode __m = std::ios_base::out | std::ios_base::in)
            : std::basic_stringstream<char_t>(__str, __m) {}
            virtual ~basic_stringstreamInsertable() {}

            basic_stringstreamInsertable(const basic_stringstreamInsertable&) = delete;
            basic_stringstreamInsertable(basic_stringstreamInsertable&& __rhs)
            : std::basic_stringstream<char_t>(std::move(__rhs)) {}

            basic_stringstreamInsertable& operator=(const basic_stringstreamInsertable&) = delete;

            basic_stringstreamInsertable& operator=(basic_stringstreamInsertable&& __rhs) {
                std::basic_stringstream<char_t>::operator=(std::move(__rhs));
                this->insertMode = __rhs.insertMode;
                return *this;
            }

            void swap(basic_stringstreamInsertable& __rhs) {
                std::basic_stringstream<char_t>::swap(__rhs);
                std::swap(this->insertMode, __rhs.insertMode);
            }

            void setInsertMode(bool insert) {
                this->insertMode = insert;
            }

            bool isInsertMode() {
                return this->insertMode;
            }
            
            #define GENERATE_STRINSTREAM_IN_OPERATOR(type) basic_stringstreamInsertable& operator<<(const type& t) { \
                if(this->insertMode) { \
                    basic_stringstreamInsertable<char_t> ss; \
                    std::string tmp = this->str(); \
                    ss << tmp.substr(0, this->tellp()); \
                    ss << t; \
                    auto posp = ss.tellp(); \
                    auto posg = ss.tellg(); \
                    ss << tmp.substr(this->tellp()); \
                    this->swap(ss); \
                    this->seekp(posp); \
                    this->seekg(posg); \
                } else { \
                    std::basic_stringstream<char_t>::operator<<(t); \
                } \
                return *this; \
            }
            
            #define GENERATE_STRINSTREAM_OUT_OPERATOR(type) basic_stringstreamInsertable& operator>>(type& t) { \
                std::basic_stringstream<char_t>::operator>>(t); \
                return *this; \
            }

            #define GENERATE_STRINSTREAM_IN_OUT_OPERATOR(type) GENERATE_STRINSTREAM_IN_OPERATOR(type) \
            GENERATE_STRINSTREAM_OUT_OPERATOR(type)

            GENERATE_STRINSTREAM_IN_OUT_OPERATOR(long)
            GENERATE_STRINSTREAM_IN_OUT_OPERATOR(unsigned long)
            GENERATE_STRINSTREAM_IN_OUT_OPERATOR(short)
            GENERATE_STRINSTREAM_IN_OUT_OPERATOR(unsigned short)
            GENERATE_STRINSTREAM_IN_OUT_OPERATOR(int)
            GENERATE_STRINSTREAM_IN_OUT_OPERATOR(unsigned int)
            GENERATE_STRINSTREAM_IN_OUT_OPERATOR(long long)
            GENERATE_STRINSTREAM_IN_OUT_OPERATOR(unsigned long long)
            GENERATE_STRINSTREAM_IN_OUT_OPERATOR(double)
            GENERATE_STRINSTREAM_IN_OUT_OPERATOR(float)
            GENERATE_STRINSTREAM_IN_OUT_OPERATOR(long double)
            GENERATE_STRINSTREAM_IN_OUT_OPERATOR(void*)
            GENERATE_STRINSTREAM_IN_OPERATOR(char_t*)
            
            basic_stringstreamInsertable& operator<<(const char_t& t) {
                if(this->insertMode) {
                    basic_stringstreamInsertable<char_t> ss;
                    std::string tmp = this->str();
                    ss << tmp.substr(0, this->tellp());
                    ss.write(&t, 1);
                    auto posp = ss.tellp();
                    auto posg = ss.tellg();
                    ss << tmp.substr(this->tellp());
                    this->swap(ss);
                    this->setInsertMode(true);
                    this->seekp(posp);
                    this->seekg(posg);
                    if(this->fail()) {
                        std::cout << " WAT?! ";
                    }
                } else {
                    std::basic_stringstream<char_t>::write(&t, 1);
                }
                return *this;
            }
            
            basic_stringstreamInsertable& operator>>(char_t& t) {
                std::basic_stringstream<char_t>::get(t);
                return *this;
            }
            
            #undef GENERATE_STRINSTREAM_IN_OUT_OPERATOR
            #undef GENERATE_STRINSTREAM_IN_OPERATOR
            #undef GENERATE_STRINSTREAM_OUT_OPERATOR

            void removeNextChar() {
                auto cutPosition = this->tellp();
                
                auto oldString = this->str();
                this->str("");
                
                *this << oldString.substr(0, cutPosition);
                
                auto posp = this->tellp();
                auto posg = this->tellg();
                
                *this << oldString.substr(cutPosition + std::ios::pos_type(1));

                this->seekp(posp);
                this->seekg(posg);
            }
            
            void removePreviousChar() {
                auto cutPosition = this->tellp();
                
                auto oldString = this->str();
                this->str("");
                
                *this << oldString.substr(0, cutPosition - std::ios::pos_type(1));
                
                auto posp = this->tellp();
                auto posg = this->tellg();
                
                *this << oldString.substr(cutPosition);
                
                this->seekp(posp);
                this->seekg(posg);
            }
        };
        
        using stringstreamInsertable = basic_stringstreamInsertable<char>;
    }
}

#endif /* STRINGSTREAM_H */

