#pragma once

#include <string>
#include <vector>

#include "HTMLTag.h"

enum LastParsePhase {
    LPP_NOTHING = 1,
    LPP_TAG_OPENED,
    LPP_TAG_OPENED_ATTRIBUTES,
    LPP_TAG_READ_ATTRIBUTE_VALUE,
    LPP_TAG_CLOSED,
    LPP_READ_VALUE
};

class XHTMLParser {
    
public:
    explicit XHTMLParser(const std::string& filename);
    void process();
    
    std::shared_ptr<HTMLTag> getRootTag() const { return m_root_tag; }
    std::vector<std::string> getPageLinks() const { return m_pageLinks; }
    
private:
    void parseClosedTag(const std::string& word);
    void processWord(const std::string& word);
    void parseTag(const std::string& word);
    void parseTagValue(const std::string& word);
    
    void parseAttributeName(const std::string& word);
    void parseAttributeValue(const std::string& word);
    
    void closedCurrentTag();
    
private:
    std::string                 m_filename;
    std::shared_ptr<HTMLTag>    m_root_tag;
    std::shared_ptr<HTMLTag>    m_current_tag;
    
    std::vector<std::string>    m_pageLinks;
    LastParsePhase              m_last_parse_phase;
    
    std::pair<std::string, std::string> m_current_parsed_attribute;
};
