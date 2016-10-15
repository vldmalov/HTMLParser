#include "FileHelper.h"

#include "boost/filesystem.hpp"
#include <iostream>

namespace FileHelper {
    
    std::vector<std::string> getDirFileNames(const std::string& dir)
    {
        std::vector<std::string> result;
        
        boost::filesystem::path dir_path( dir );
        if ( !exists( dir_path ) )
        {
            std::cout << "Directory " << dir_path.native() << " not exist" << std::endl;
            return std::vector<std::string>();
        }
        
        for ( boost::filesystem::directory_iterator it(dir_path), end_it; it != end_it; ++it )
        {
            if ( is_regular_file(it->status()) )
            {
                boost::filesystem::path current_path = it->path();
                boost::filesystem::path filename = current_path.filename();
                
                result.push_back(filename.native());
            }
        }
        
        return result;
    }
    
    std::string getFullFileName(const std::string& dir, const std::string& fileName)
    {
        boost::filesystem::path dir_path( dir );
        dir_path.append(fileName);
        
        return dir_path.native();
    }
    
}
