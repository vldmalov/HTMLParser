#include "XHTMLParser.h"

#include <cassert>
#include <iostream>
#include <fstream>

#include "HTMLTag.h"

XHTMLParser::XHTMLParser()
: m_rootTag(new HTMLTag())
{
    m_rootTag->name = "Document";
}

void XHTMLParser::processFile(const std::string& filename)
{
    m_pageLinks.clear();
    std::ifstream inputStream(filename);
    if(inputStream.is_open()) {
        
        std::string currentWord;
        std::shared_ptr<HTMLTag> currentTag = m_rootTag;
        LastParsePhase lastPhase = LPP_NOTHING;
        while(inputStream >> currentWord) {
            processWord(currentWord, lastPhase, currentTag);
        }
    }
    inputStream.close();
}

void XHTMLParser::processWord(const std::string& word, LastParsePhase& lastPhase, std::shared_ptr<HTMLTag>& currentTag)
{
//    std::cout << "* processWord. word = " << word << std::endl;
//    std::cout << "* processWord. phase = " << lastPhase << std::endl;
    if(word.empty()) {
        return;
    }
    
    switch (lastPhase) {
        case LPP_NOTHING:
            parseTag(word, lastPhase, currentTag);
            break;
            
        case LPP_TAG_OPENED:
            if(word[0] == '<') {
                if(word[1] == '/') {
                    parseClosedTag(word, lastPhase, currentTag);
                }
                else {
                    parseTag(word, lastPhase, currentTag);
                }
            }
            else {
                parseTagValue(word, lastPhase, currentTag);
            }
            break;
            
        case LPP_READ_VALUE:
            parseTagValue(word, lastPhase, currentTag);
            break;
            
        case LPP_TAG_CLOSED:
            if(word[0] == '<') {
                if(word[1] == '/') {
                    parseClosedTag(word, lastPhase, currentTag);
                }
                else {
                    parseTag(word, lastPhase, currentTag);
                }
            }
            break;
            
        case LPP_TAG_OPENED_ATTRIBUTES:
            if(word[0] == '>') {
                lastPhase = LPP_TAG_OPENED;
                
                const size_t closeTagSize = std::string(">").size();
                std::string remainWord = word.substr(closeTagSize);
                if(!remainWord.empty()) {
                    processWord(remainWord, lastPhase, currentTag);
                }
            }
            else {
                parseAttributeName(word, lastPhase, currentTag);
            }
            break;
            
        case LPP_TAG_READ_ATTRIBUTE_VALUE:
            parseAttributeValue(word, lastPhase, currentTag);
            
        default:
            break;
    }
}

void XHTMLParser::parseTag(const std::string& word, LastParsePhase& lastPhase, std::shared_ptr<HTMLTag>& currentTag)
{
    assert(word[0] == '<');
    
    const size_t openTagSize = std::string("<").size();
    
    size_t closeTagPosition = word.find(">");
    std::string tagName;
    std::string remainWord;
    
    if(closeTagPosition != std::string::npos) {
        tagName = word.substr(openTagSize, closeTagPosition - openTagSize);
        remainWord = word.substr(closeTagPosition + 1);
        lastPhase = LPP_TAG_OPENED;
    }
    else
    {
        tagName = word.substr(openTagSize);
        lastPhase = LPP_TAG_OPENED_ATTRIBUTES;
    }
    
    std::shared_ptr<HTMLTag> newTag(new HTMLTag());
    newTag->name = tagName;
    newTag->parent = currentTag;
    currentTag->childs.push_back(newTag);
    
    currentTag = newTag;
    
    if(!remainWord.empty()) {
        processWord(remainWord, lastPhase, currentTag);
    }
}

void XHTMLParser::parseClosedTag(const std::string& word, LastParsePhase& lastPhase, std::shared_ptr<HTMLTag>& currentTag)
{
    assert(word.front() == '<' && *(++word.begin()) == '/');
    
    size_t closeTagPosition = word.find(">");
    const size_t openTagSize = std::string("</").size();
    const std::string closeTagName = word.substr(openTagSize, closeTagPosition - openTagSize);
    const std::string remainWord = word.substr(closeTagPosition + 1);
    assert(currentTag->name == closeTagName);
    
    closedCurrentTag(lastPhase, currentTag);
    
    if(!remainWord.empty()) {
        processWord(remainWord, lastPhase, currentTag);
    }
}

void XHTMLParser::closedCurrentTag(LastParsePhase& lastPhase, std::shared_ptr<HTMLTag>& currentTag)
{
    currentTag = std::shared_ptr<HTMLTag>(currentTag->parent);
    lastPhase = LPP_TAG_CLOSED;
}

void XHTMLParser::parseTagValue(const std::string& word, LastParsePhase& lastPhase, std::shared_ptr<HTMLTag>& currentTag)
{
    lastPhase = LPP_READ_VALUE;
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
    
    if(!value.empty() && !currentTag->value.empty()) {
        currentTag->value.append(" ");
    }
    currentTag->value.append(value);
    
    if(!remainWord.empty()) {
        parseClosedTag(remainWord, lastPhase, currentTag);
    }
}

void XHTMLParser::parseAttributeName(const std::string& word, LastParsePhase& lastPhase, std::shared_ptr<HTMLTag>& currentTag)
{
    size_t paramSeparatorPos = word.find("=");
    assert(paramSeparatorPos != std::string::npos);
    
    std::string attributeName = word.substr(0, paramSeparatorPos);
    assert(!attributeName.empty());
    
    m_currentParsedAttribute.first = attributeName;
    m_currentParsedAttribute.second.clear();
    
    lastPhase = LPP_TAG_READ_ATTRIBUTE_VALUE;
    
    std::string remainWord = word.substr(paramSeparatorPos + 1);
    if(!remainWord.empty()) {
        processWord(remainWord, lastPhase, currentTag);
    }
}

void XHTMLParser::parseAttributeValue(const std::string& word, LastParsePhase& lastPhase, std::shared_ptr<HTMLTag>& currentTag)
{
    std::string parseWord;
    std::string remainWord;
    
    if(m_currentParsedAttribute.second.empty()) {
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
        lastPhase = LPP_TAG_OPENED_ATTRIBUTES;
    }
    
    if(!m_currentParsedAttribute.second.empty()) {
        m_currentParsedAttribute.second.append(" ");
    }
    m_currentParsedAttribute.second.append(attributeValue);
    
    if(lastPhase == LPP_TAG_OPENED_ATTRIBUTES) {
        currentTag->attributes.push_back(m_currentParsedAttribute);
        
        if(m_currentParsedAttribute.first == "href") {
            m_pageLinks.push_back(m_currentParsedAttribute.second);
        }
    }
    
    if(!remainWord.empty()) {
        processWord(remainWord, lastPhase, currentTag);
    }

}
