#ifndef READFILE_H
#define READFILE_H

#include <sstream>
#include <fstream>
#include "../IOException.h"
#include <boost/algorithm/string/predicate.hpp>

namespace engine {
    namespace util {
        using namespace std;
        string readFile(string inputFile, string ending = "");
        string getFileNameExtension(string fileName);
    } 
}


#endif /* READFILE_H */

