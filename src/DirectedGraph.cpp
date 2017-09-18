#include "DirectedGraph.h"

#include <fstream>
#include <sstream>
#include <vector>

void DirectedGraph::addEdge(const std::string& nodeIdFrom, const std::string& nodeIdTo)
{
    m_nodes[nodeIdFrom].NodeIdsTo.insert(nodeIdTo);
    m_nodes[nodeIdTo].NodeIdsFrom.insert(nodeIdFrom);
}

bool DirectedGraph::hasDirectEdge(const std::string& nodeIdFrom, const std::string& nodeIdTo) const
{
    NodesMap::const_iterator itFromNodeId = m_nodes.find(nodeIdFrom);
    if(itFromNodeId == m_nodes.end()) {
        return false;
    }
    
    const std::unordered_set<std::string>& nodeIdsTo = itFromNodeId->second.NodeIdsTo;
    std::unordered_set<std::string>::const_iterator itFoundNodeTo = nodeIdsTo.find(nodeIdTo);
    
    return itFoundNodeTo != nodeIdsTo.end();
}

bool DirectedGraph::hasLessThenTwoEdgeWay(const std::string& nodeIdFrom, const std::string& nodeIdTo) const
{
    NodesMap::const_iterator itFromNodeId = m_nodes.find(nodeIdFrom);
    NodesMap::const_iterator itToNodeId = m_nodes.find(nodeIdTo);
    if(itFromNodeId == m_nodes.end() || itToNodeId == m_nodes.end()) {
        return false; // One of nodes is not exist
    }
    
    const std::unordered_set<std::string>& nodeIdsFromFirstNode = itFromNodeId->second.NodeIdsTo;
    std::unordered_set<std::string>::const_iterator itFoundNodeTo = nodeIdsFromFirstNode.find(nodeIdTo);
    
    if(itFoundNodeTo != nodeIdsFromFirstNode.end()) {
        return true;  // Has found direct way
    }
    
    const std::unordered_set<std::string>& nodeIdsToSecondNode = itToNodeId->second.NodeIdsFrom;
    
    for(const std::string& interimNodeId : nodeIdsFromFirstNode) {
        if(nodeIdsToSecondNode.find(interimNodeId) != nodeIdsToSecondNode.end()) {
            return true;
        }
    }
    
    return false;
}

void DirectedGraph::saveToFile(const std::string& fileName) const
{
    std::ofstream fileStream;
    fileStream.open(fileName, std::ios::out | std::ios::trunc);
    
    for(NodesMap::const_iterator it = m_nodes.begin(), it_end = m_nodes.end(); it != it_end; ++it) {
        
        const std::string nodeName = it->first;
        fileStream << nodeName << " ";
        const std::unordered_set<std::string>& nodeIdsTo = it->second.NodeIdsTo;
        for(const std::string& nodeToId : nodeIdsTo) {
            fileStream << nodeToId << " ";
        }
        fileStream << std::endl;
    }
    
    fileStream.close();
}

void DirectedGraph::loadFromFile(const std::string& fileName)
{
    m_nodes.clear();
    
    std::ifstream fileStream;
    fileStream.open(fileName, std::ios::in);
    
    std::string currentLine;
    while(std::getline(fileStream, currentLine))
    {
        std::istringstream iss(currentLine);
        std::vector<std::string> nodeIds( std::istream_iterator<std::string>{iss},
                                          std::istream_iterator<std::string>());
        
        std::string currentNodeId = nodeIds.front();
        for(std::vector<std::string>::const_iterator it = nodeIds.begin() + 1, it_end = nodeIds.end(); it != it_end; ++it)
        {
            addEdge(currentNodeId, *it);
        }
    }
    
    fileStream.close();
}
