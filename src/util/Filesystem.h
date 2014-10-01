#pragma once
/*

Filesystem.h - FileSystemHandler

A utility class for working with the filesystem.

 */

#include <boost/filesystem.hpp>
#include <string>

class FileSystemHandler {

public:

    bool CreateDirectory(std::string directory);
    bool CheckIfExists(std::string file);

    std::string expand_user(std::string path);


private:



};