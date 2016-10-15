#include "DirectedGraph.h"

void DirectedGraph::addEdge(const std::string nodeIdFrom, const std::string nodeIdTo)
{
    NodesMap::const_iterator foundFromNodeId = m_nodes.find(nodeIdFrom);
    if(foundFromNodeId == m_nodes.end()) {
        m_nodes.insert(std::make_pair(nodeIdFrom, GraphNode()));
    }
    m_nodes[nodeIdFrom].NodeIdsTo.insert(nodeIdTo);
    
    NodesMap::const_iterator foundToNodeId = m_nodes.find(nodeIdTo);
    if(foundToNodeId == m_nodes.end()) {
        m_nodes.insert(std::make_pair(nodeIdTo, GraphNode()));
    }
    m_nodes[nodeIdTo].NodeIdsFrom.insert(nodeIdFrom);
}

bool DirectedGraph::hasDirectEdge(const std::string nodeIdFrom, const std::string nodeIdTo) const
{
    NodesMap::const_iterator itFromNodeId = m_nodes.find(nodeIdFrom);
    if(itFromNodeId == m_nodes.end()) {
        return false;
    }
    
    const std::unordered_set<std::string>& nodeIdsTo = itFromNodeId->second.NodeIdsTo;
    std::unordered_set<std::string>::const_iterator itFoundNodeTo = nodeIdsTo.find(nodeIdTo);
    
    return itFoundNodeTo != nodeIdsTo.end();
}

bool DirectedGraph::hasLessThenTwoEdgeWay(const std::string nodeIdFrom, const std::string nodeIdTo) const
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
