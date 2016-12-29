#include "readFile.h"

namespace engine {
    namespace util {
        using namespace std;
        string readFile(string inputFile, string ending) {
            if(ending.length()>0 && !boost::algorithm::ends_with(inputFile, ending)) {
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
        
        string getFileNameExtension(string fileName) {
            std::stringstream buffer;
            for(auto rit = fileName.rbegin(); rit!=fileName.rend(); ++rit) {
                buffer << *rit;
            }
            return buffer.str();
        }
    } 
}