#pragma once

#include <string>
#include <vector>
#include <iostream>

struct HTMLTag {
    std::string name;
    std::string value;
    std::vector<std::pair<std::string, std::string>> attributes;
    std::vector<std::shared_ptr<HTMLTag>> childs;
    std::weak_ptr<HTMLTag> parent;
    
    friend std::ostream& operator<<(std::ostream &output, const HTMLTag& tag);
private:
    
    void outputImpl(std::ostream &os, unsigned nestingLevel) const;
};
