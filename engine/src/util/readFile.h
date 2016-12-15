#ifndef READFILE_H
#define READFILE_H

#include <sstream>
#include <fstream>
#include "../IOException.h"
#include <boost/algorithm/string/predicate.hpp>

namespace engine {
    namespace util {
        using namespace std;
        string readFile(string inputFile, string ending="") {
            if(!boost::algorithm::ends_with(inputFile, ending)) {
                string err = "Wrong file extension on " + inputFile + ". Expected " + ending + ".";
                throw IOException(err.c_str());
            }
            
            try {
                std::ifstream ifstr(inputFile);
                std::stringstream buffer;
                buffer << ifstr.rdbuf();
                return buffer.str();
            } catch (const Exception &e) {
                string err = "Could not read file " + inputFile + ": " + e.what();
                throw IOException(err.c_str());
            }
        }
    }
}


#endif /* READFILE_H */

