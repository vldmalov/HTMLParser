#include <iostream>
#include <vector>
#include <cassert>
#include <cstdio>

#include "XHTMLParser.h"
#include "FileHelper.h"

const std::string DEFAULT_DIR_NAME = "../../examples";

int main(int argc, const char * argv[]) {
    
    std::string dirName = DEFAULT_DIR_NAME;
    std::vector<std::string> fileNames = FileHelper::getDirFileNames(dirName, "html");
    if(fileNames.empty()) {
        std::cout << "Files not found! For proccessing required at least one xHTML file" << std::endl;
        exit(1);
    }
    
    for(const std::string fileName : fileNames)
    {
        std::cout << fileName << std::endl;
        
        std::string fullFileName = FileHelper::getFullFileName(dirName, fileName);
        
        std::shared_ptr<XHTMLParser> parser(new XHTMLParser(fullFileName));
        parser->process();
        
        std::cout << "RESULT: " << std::endl;
//        std::shared_ptr<HTMLTag> rootTag = parser->getRootTag();
//        std::cout << *rootTag.get();
        
        std::vector<std::string> pageLinks = parser->getPageLinks();
        std::cout << "Page links :" << std::endl;
        for(const std::string& link : pageLinks) {
            std::cout << " " << link << std::endl;
        }
    }
    std::cout << std::endl;
    
    
    return 0;
}
