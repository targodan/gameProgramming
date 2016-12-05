#ifndef STRINGSTREAM_H
#define STRINGSTREAM_H

#include <mutex>
#include <sstream>
#include <utility>

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
      
            template<class T>
            basic_stringstreamTS& operator<<(const T& t) {
                std::unique_lock<std::mutex> lock(this->mutex);
                std::basic_stringstream<char_t>::operator<<(t);
                return *this;
            }
            
            template<class T>
            basic_stringstreamTS& operator>>(T& t) {
                std::unique_lock<std::mutex> lock(this->mutex);
                std::basic_stringstream<char_t>::operator>>(t);
                return *this;
            }
            
            std::basic_string<char_t> str() {
                std::unique_lock<std::mutex> lock(this->mutex);
                return std::basic_stringstream<char_t>::str();
            }
            
            void str(std::basic_string<char_t>& s) {
                std::unique_lock<std::mutex> lock(this->mutex);
                return std::basic_stringstream<char_t>::str(s);
            }
            
            std::basic_string<char_t> flush() {
                std::unique_lock<std::mutex> lock(this->mutex);
                std::basic_string<char_t> ret = std::basic_stringstream<char_t>::str();
                std::basic_stringstream<char_t>::flush();
                return ret;
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
    }
}

#endif /* STRINGSTREAM_H */

