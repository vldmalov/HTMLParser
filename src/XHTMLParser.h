#pragma once

#include <string>
#include <vector>

struct HTMLTag;

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
    XHTMLParser();
    void processFile(const std::string& filename);
    
    std::shared_ptr<HTMLTag> getRootTag() const { return m_rootTag; }
    std::vector<std::string> getPageLinks() const { return m_pageLinks; }
    
private:
    void parseClosedTag     (const std::string& word, LastParsePhase& lastPhase, std::shared_ptr<HTMLTag>& currentTag);
    void processWord        (const std::string& word, LastParsePhase& lastPhase, std::shared_ptr<HTMLTag>& currentTag);
    void parseTag           (const std::string& word, LastParsePhase& lastPhase, std::shared_ptr<HTMLTag>& currentTag);
    void parseTagValue      (const std::string& word, LastParsePhase& lastPhase, std::shared_ptr<HTMLTag>& currentTag);
    
    void parseAttributeName (const std::string& word, LastParsePhase& lastPhase, std::shared_ptr<HTMLTag>& currentTag);
    void parseAttributeValue(const std::string& word, LastParsePhase& lastPhase, std::shared_ptr<HTMLTag>& currentTag);
    
    void closedCurrentTag(LastParsePhase& lastPhase, std::shared_ptr<HTMLTag>& currentTag);
    
    XHTMLParser(const XHTMLParser&);
    XHTMLParser& operator=(const XHTMLParser&);
    
private:
    std::shared_ptr<HTMLTag>    m_rootTag;
    std::vector<std::string>    m_pageLinks;
    std::pair<std::string, std::string> m_currentParsedAttribute;
};
