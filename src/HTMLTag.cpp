#include "HTMLTag.h"

std::ostream& operator<<(std::ostream &os, const HTMLTag& outTag)
{
    outTag.outputImpl(os, 0);
    return os;
}

void HTMLTag::outputImpl(std::ostream &os, unsigned nestingLevel) const
{
    std::string levelIndent(4 * nestingLevel, ' ');
    os << levelIndent << "Tag name: " << name << std::endl;
    os << levelIndent << "  " << "Tag value: ";
    if(!value.empty()) {
        os << value << std::endl;
    } else {
        os << "none" << std::endl;
    }
    
    os << levelIndent << "  " << "Tag attributes: ";
    if(attributes.empty()) {
        os << "none";
    }
    os << std::endl;
    for(const std::pair<std::string, std::string>& attribute : attributes) {
        os << levelIndent << "    " << attribute.first << ": " << attribute.second << std::endl;
    }
    
    os << levelIndent << "  " << "Tag childs: ";
    if(childs.empty()) {
        os << "none";
    }
    os << std::endl;
    for(std::shared_ptr<HTMLTag> child : childs) {
        const HTMLTag& childTag = *child.get();
        childTag.outputImpl(os, nestingLevel + 1);
    }
}
