#include "readFile.h"
#include <unistd.h>

namespace engine {
    namespace util {
        using namespace std;
        
        const unsigned int MAX_PATH_LENGTH = 1024;
        string readFile(string inputFile, string ending) {
            if(ending.length()>0 && !boost::algorithm::ends_with(inputFile, ending)) {
                string err = "Wrong file extension on " + inputFile + ". Expected " + ending + ".";
                throw IOException(err.c_str());
            }
            
            char currentDirectory[MAX_PATH_LENGTH];
            if(getcwd(currentDirectory, sizeof(currentDirectory)) == NULL) {
                string err = "Error while opening " + inputFile + ": Could not read current directory.";
                throw IOException(err.c_str());
            }
            
            std::ifstream ifstr(inputFile);

            if(!ifstr.good()) {
                string err = "File " + inputFile + " does not exist.";
                throw IOException(err.c_str());
            }
            
            try {    
                std::stringstream buffer;
                buffer << ifstr.rdbuf();
                return buffer.str();
            } catch (const Exception &e) {
                string err = "Could not read file " + inputFile + ": " + e.what();
                throw IOException(err.c_str());
            }
        }
        
        string getFileNameExtension(string fileName) {
            return fileName.substr(fileName.find_last_of(".")+1);
        }
        
        string getAbsoluteFromRelativePath(string relativePath) {
            // FIXME: This is highly instable!
            // Example inputs that create questionable outputs:
            // "/already/abs/path"
            // "./relative/path"
            // "../../parentDirectories"
            char currentDirectory[MAX_PATH_LENGTH];
            if(getcwd(currentDirectory, sizeof(currentDirectory)) == NULL) {
                string err = "Error while getting absolute from relative path: could not read current directory.";
                throw IOException(err.c_str());
            }
            
            return string(currentDirectory) + "/" + relativePath;
        }
    } 
}