#include "XHTMLParser.h"

#include <cassert>
#include <iostream>
#include <fstream>

#include "HTMLTag.h"

XHTMLParser::XHTMLParser(const std::string& filename)
: m_filename(filename)
, m_root_tag(new HTMLTag())
, m_last_parse_phase(LPP_NOTHING)
{
    m_root_tag->name = "Document";
    m_current_tag = m_root_tag;
}

void XHTMLParser::process()
{
    std::ifstream inputStream(m_filename);
    if(inputStream.is_open()) {
        
        std::string currentWord;
        while(inputStream >> currentWord) {
            processWord(currentWord);
        }
    }
    inputStream.close();
}

void XHTMLParser::processWord(const std::string& word)
{
//    std::cout << "* processWord. word = " << word << std::endl;
//    std::cout << "* processWord. phase = " << m_last_parse_phase << std::endl;
    if(word.empty()) {
        return;
    }
    
    if(m_last_parse_phase == LPP_NOTHING) {
        parseTag(word);
    }
    else if(m_last_parse_phase == LPP_TAG_OPENED) {
        if(word[0] == '<') {
            if(word[1] == '/') {
                parseClosedTag(word);
            }
            else {
                parseTag(word);
            }
        }
        else {
            parseTagValue(word);
        }
    }
    else if(m_last_parse_phase == LPP_READ_VALUE) {
        parseTagValue(word);
    }
    else if(m_last_parse_phase == LPP_TAG_CLOSED) {
        if(word[0] == '<') {
            if(word[1] == '/') {
                parseClosedTag(word);
            }
            else {
                parseTag(word);
            }
        }
    }
    else if(m_last_parse_phase == LPP_TAG_OPENED_ATTRIBUTES) {
        if(word[0] == '>') {
            m_last_parse_phase = LPP_TAG_OPENED;
            
            const size_t closeTagSize = std::string(">").size();
            std::string remainWord = word.substr(closeTagSize);
            if(!remainWord.empty()) {
                processWord(remainWord);
            }
        }
        else {
            parseAttributeName(word);
        }
    }
    else if(m_last_parse_phase == LPP_TAG_READ_ATTRIBUTE_VALUE) {
        parseAttributeValue(word);
    }

}

void XHTMLParser::parseTag(const std::string& word)
{
    assert(word[0] == '<');
    
    const size_t openTagSize = std::string("<").size();
    
    size_t closeTagPosition = word.find(">");
    std::string tagName;
    std::string remainWord;
    
    if(closeTagPosition != std::string::npos) {
        tagName = word.substr(openTagSize, closeTagPosition - openTagSize);
        remainWord = word.substr(closeTagPosition + 1);
        m_last_parse_phase = LPP_TAG_OPENED;
    }
    else
    {
        tagName = word.substr(openTagSize);
        m_last_parse_phase = LPP_TAG_OPENED_ATTRIBUTES;
    }
    
    std::shared_ptr<HTMLTag> newTag(new HTMLTag());
    newTag->name = tagName;
    newTag->parent = m_current_tag;
    m_current_tag->childs.push_back(newTag);
    
    m_current_tag = newTag;
    
    if(!remainWord.empty()) {
        processWord(remainWord);
    }
}

void XHTMLParser::parseClosedTag(const std::string& word)
{
    assert(word.front() == '<' && *(++word.begin()) == '/');
    
    size_t closeTagPosition = word.find(">");
    const size_t openTagSize = std::string("</").size();
    const std::string closeTagName = word.substr(openTagSize, closeTagPosition - openTagSize);
    const std::string remainWord = word.substr(closeTagPosition + 1);
    assert(m_current_tag->name == closeTagName);
    
    closedCurrentTag();
    
    if(!remainWord.empty()) {
        processWord(remainWord);
    }
}

void XHTMLParser::closedCurrentTag()
{
    m_current_tag = std::shared_ptr<HTMLTag>(m_current_tag->parent);
    m_last_parse_phase = LPP_TAG_CLOSED;
}

void XHTMLParser::parseTagValue(const std::string& word)
{
    m_last_parse_phase = LPP_READ_VALUE;
    std::string value;
    std::string remainWord;
    
    size_t closeTagPosition = word.find("</");
    if(closeTagPosition == std::string::npos) {
        value = word;
    }
    else {
        value = word.substr(0, closeTagPosition);
        remainWord = word.substr(closeTagPosition);
    }
    
    if(!value.empty() && !m_current_tag->value.empty()) {
        m_current_tag->value.append(" ");
    }
    m_current_tag->value.append(value);
    
    if(!remainWord.empty()) {
        parseClosedTag(remainWord);
    }
}

void XHTMLParser::parseAttributeName(const std::string& word)
{
    size_t paramSeparatorPos = word.find("=");
    assert(paramSeparatorPos != std::string::npos);
    
    std::string attributeName = word.substr(0, paramSeparatorPos);
    assert(!attributeName.empty());
    
    m_current_parsed_attribute.first = attributeName;
    m_current_parsed_attribute.second.clear();
    
    m_last_parse_phase = LPP_TAG_READ_ATTRIBUTE_VALUE;
    
    std::string remainWord = word.substr(paramSeparatorPos + 1);
    if(!remainWord.empty()) {
        processWord(remainWord);
    }
}

void XHTMLParser::parseAttributeValue(const std::string& word)
{
    std::string parseWord;
    std::string remainWord;
    
    if(m_current_parsed_attribute.second.empty()) {
        assert(word.front() == '\"');
        parseWord = word.substr(1);
    } else {
        parseWord = word;
    }
    
    std::string attributeValue;
    size_t closedQuotePosition = parseWord.find("\"");
    if(closedQuotePosition == std::string::npos) {
        attributeValue = parseWord;
    } else {
        attributeValue = parseWord.substr(0, closedQuotePosition);
        remainWord = parseWord.substr(closedQuotePosition + 1);
        m_last_parse_phase = LPP_TAG_OPENED_ATTRIBUTES;
    }
    
    if(!m_current_parsed_attribute.second.empty()) {
        m_current_parsed_attribute.second.append(" ");
    }
    m_current_parsed_attribute.second.append(attributeValue);
    
    if(m_last_parse_phase == LPP_TAG_OPENED_ATTRIBUTES) {
        m_current_tag->attributes.push_back(m_current_parsed_attribute);
        
        if(m_current_parsed_attribute.first == "href") {
            m_pageLinks.push_back(m_current_parsed_attribute.second);
        }
    }
    
    if(!remainWord.empty()) {
        processWord(remainWord);
    }

}
