#pragma once

#include <string>
#include <unordered_set>
#include <unordered_map>

struct GraphNode {
    std::unordered_set<std::string> NodeIdsTo;
    std::unordered_set<std::string> NodeIdsFrom;
};

typedef std::unordered_map<std::string, GraphNode> NodesMap;

class DirectedGraph {

public:
    void addEdge(const std::string& nodeIdFrom, const std::string& nodeIdTo);
    bool hasDirectEdge(const std::string& nodeIdFrom, const std::string& nodeIdTo) const;
    bool hasLessThenTwoEdgeWay(const std::string& nodeIdFrom, const std::string& nodeIdTo) const;
    
    void saveToFile(const std::string& fileName) const;
    void loadFromFile(const std::string& fileName);
    
private:
    NodesMap m_nodes;
};
