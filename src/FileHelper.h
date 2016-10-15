#pragma once

#include <string>
#include <vector>

namespace FileHelper {

    std::vector<std::string> getDirFileNames(const std::string& dir);
    std::string getFullFileName(const std::string& dir, const std::string& fileName);
}
