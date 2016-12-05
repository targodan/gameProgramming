#ifndef COMMAND_H
#define COMMAND_H

#include <iostream>
#include <string>
#include <sstream>
#include "../util/vector.h"
#include "../InvalidConversionException.h"

namespace engine {
    namespace IO {
        using engine::util::vector;
        
        class Command {
        protected:
            template<class T>
            T strTo(const std::string& str) const {
                T ret;
                if(!(std::istringstream(str) >> ret)) {
                    throw InvalidConversionException("\"%s\" could not be converted to type %s.", str.c_str(), typeid(T).name());
                }
                return ret;
            }
            
        public:
            Command() {}
            virtual ~Command() {}
            
            virtual std::string getName() const = 0;
            virtual int main(const vector<std::string>& args, std::istream& stdin, std::ostream& stdout, std::ostream& stderr) const = 0;
        };
    }
}

#endif /* COMMAND_H */

