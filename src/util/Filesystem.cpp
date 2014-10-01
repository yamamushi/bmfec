#include "Filesystem.h"
#include "logger.h"
#include <iostream>
#include <cassert>

bool FileSystemHandler::CreateDirectory(std::string directory) {

    directory = expand_user(directory);

    if(CheckIfExists(directory)){
        GlobalLogger::Instance()->writeToLogFile("Could not create directory: " + directory + " - File already exists");
        return false;
    }
    else{

        bool created = boost::filesystem::create_directory(directory);
        if(created) {
            GlobalLogger::Instance()->writeToLogFile("Directory created: " + directory);
        }
        else{
            return false;
        }
    }
}

bool FileSystemHandler::CheckIfExists(std::string file) {
    file = expand_user(file);
    return boost::filesystem::exists(file);
}


std::string FileSystemHandler::expand_user(std::string path) {
    if (! path.empty() && path[0] == '~') {
        assert(path.size() == 1 || path[1] == '/');  // or other error handling
        char const* home = getenv("HOME");
        if (home || ((home = getenv("USERPROFILE")))) {
            path.replace(0, 1, home);
        }
        else {
            char const *hdrive = getenv("HOMEDRIVE"),
                    *hpath = getenv("HOMEPATH");
            assert(hdrive);  // or other error handling
            assert(hpath);
            path.replace(0, 1, std::string(hdrive) + hpath);
        }
    }
    return path;
}