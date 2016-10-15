#include <iostream>
#include <vector>
#include <cassert>
#include <cstdio>

#include "XHTMLParser.h"
#include "FileHelper.h"
#include "DirectedGraph.h"

const std::string DEFAULT_DIR_NAME = "../../examples";

void checkGraphData(std::shared_ptr<DirectedGraph> graph);

int main(int argc, const char * argv[]) {
    
    std::string dirName = DEFAULT_DIR_NAME;
    std::vector<std::string> fileNames = FileHelper::getDirFileNames(dirName, "html");
    if(fileNames.empty()) {
        std::cout << "Files not found! For proccessing required at least one xHTML file" << std::endl;
        exit(1);
    }
    
    std::shared_ptr<DirectedGraph> graph(new DirectedGraph());
    
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
            graph->addEdge(fileName, link);
        }
    }
    std::cout << std::endl;
    
    checkGraphData(graph);
    
    return 0;
}

void checkGraphData(std::shared_ptr<DirectedGraph> graph)
{
    // Check all graph nodes with all on
    // 1) direct link
    // 2) 2 or less edges
    assert(graph->hasDirectEdge("page1.html", "page2.html") == true);
    assert(graph->hasDirectEdge("page1.html", "page3.html") == true);
    assert(graph->hasDirectEdge("page1.html", "page4.html") == true);
    assert(graph->hasDirectEdge("page1.html", "page5.html") == true);
    assert(graph->hasDirectEdge("page1.html", "page6.html") == false);
    assert(graph->hasDirectEdge("page1.html", "page7.html") == false);
    
    assert(graph->hasDirectEdge("page2.html", "page1.html") == false);
    assert(graph->hasDirectEdge("page2.html", "page3.html") == true);
    assert(graph->hasDirectEdge("page2.html", "page4.html") == false);
    assert(graph->hasDirectEdge("page2.html", "page5.html") == false);
    assert(graph->hasDirectEdge("page2.html", "page6.html") == false);
    assert(graph->hasDirectEdge("page2.html", "page7.html") == false);
    
    assert(graph->hasDirectEdge("page3.html", "page1.html") == false);
    assert(graph->hasDirectEdge("page3.html", "page2.html") == false);
    assert(graph->hasDirectEdge("page3.html", "page4.html") == false);
    assert(graph->hasDirectEdge("page3.html", "page5.html") == false);
    assert(graph->hasDirectEdge("page3.html", "page6.html") == true);
    assert(graph->hasDirectEdge("page3.html", "page7.html") == false);
    
    assert(graph->hasDirectEdge("page4.html", "page1.html") == false);
    assert(graph->hasDirectEdge("page4.html", "page2.html") == false);
    assert(graph->hasDirectEdge("page4.html", "page3.html") == false);
    assert(graph->hasDirectEdge("page4.html", "page5.html") == true);
    assert(graph->hasDirectEdge("page4.html", "page6.html") == false);
    assert(graph->hasDirectEdge("page4.html", "page7.html") == false);
    
    assert(graph->hasDirectEdge("page5.html", "page1.html") == false);
    assert(graph->hasDirectEdge("page5.html", "page2.html") == false);
    assert(graph->hasDirectEdge("page5.html", "page3.html") == false);
    assert(graph->hasDirectEdge("page5.html", "page4.html") == false);
    assert(graph->hasDirectEdge("page5.html", "page6.html") == true);
    assert(graph->hasDirectEdge("page5.html", "page7.html") == true);
    
    assert(graph->hasDirectEdge("page6.html", "page1.html") == false);
    assert(graph->hasDirectEdge("page6.html", "page2.html") == false);
    assert(graph->hasDirectEdge("page6.html", "page3.html") == false);
    assert(graph->hasDirectEdge("page6.html", "page4.html") == false);
    assert(graph->hasDirectEdge("page6.html", "page5.html") == false);
    assert(graph->hasDirectEdge("page6.html", "page7.html") == false);
    
    assert(graph->hasDirectEdge("page7.html", "page1.html") == true);
    assert(graph->hasDirectEdge("page7.html", "page2.html") == false);
    assert(graph->hasDirectEdge("page7.html", "page3.html") == false);
    assert(graph->hasDirectEdge("page7.html", "page4.html") == false);
    assert(graph->hasDirectEdge("page7.html", "page5.html") == false);
    assert(graph->hasDirectEdge("page7.html", "page6.html") == false);
    
    assert(graph->hasLessThenTwoEdgeWay("page1.html", "page2.html") == true);
    assert(graph->hasLessThenTwoEdgeWay("page1.html", "page3.html") == true);
    assert(graph->hasLessThenTwoEdgeWay("page1.html", "page4.html") == true);
    assert(graph->hasLessThenTwoEdgeWay("page1.html", "page5.html") == true);
    assert(graph->hasLessThenTwoEdgeWay("page1.html", "page6.html") == true);
    assert(graph->hasLessThenTwoEdgeWay("page1.html", "page7.html") == true);
    
    assert(graph->hasLessThenTwoEdgeWay("page2.html", "page1.html") == false);
    assert(graph->hasLessThenTwoEdgeWay("page2.html", "page3.html") == true);
    assert(graph->hasLessThenTwoEdgeWay("page2.html", "page4.html") == false);
    assert(graph->hasLessThenTwoEdgeWay("page2.html", "page5.html") == false);
    assert(graph->hasLessThenTwoEdgeWay("page2.html", "page6.html") == true);
    assert(graph->hasLessThenTwoEdgeWay("page2.html", "page7.html") == false);
    
    assert(graph->hasLessThenTwoEdgeWay("page3.html", "page1.html") == false);
    assert(graph->hasLessThenTwoEdgeWay("page3.html", "page2.html") == false);
    assert(graph->hasLessThenTwoEdgeWay("page3.html", "page4.html") == false);
    assert(graph->hasLessThenTwoEdgeWay("page3.html", "page5.html") == false);
    assert(graph->hasLessThenTwoEdgeWay("page3.html", "page6.html") == true);
    assert(graph->hasLessThenTwoEdgeWay("page3.html", "page7.html") == false);
    
    assert(graph->hasLessThenTwoEdgeWay("page4.html", "page1.html") == false);
    assert(graph->hasLessThenTwoEdgeWay("page4.html", "page2.html") == false);
    assert(graph->hasLessThenTwoEdgeWay("page4.html", "page3.html") == false);
    assert(graph->hasLessThenTwoEdgeWay("page4.html", "page5.html") == true);
    assert(graph->hasLessThenTwoEdgeWay("page4.html", "page6.html") == true);
    assert(graph->hasLessThenTwoEdgeWay("page4.html", "page7.html") == true);
    
    assert(graph->hasLessThenTwoEdgeWay("page5.html", "page1.html") == true);
    assert(graph->hasLessThenTwoEdgeWay("page5.html", "page2.html") == false);
    assert(graph->hasLessThenTwoEdgeWay("page5.html", "page3.html") == false);
    assert(graph->hasLessThenTwoEdgeWay("page5.html", "page4.html") == false);
    assert(graph->hasLessThenTwoEdgeWay("page5.html", "page6.html") == true);
    assert(graph->hasLessThenTwoEdgeWay("page5.html", "page7.html") == true);
    
    assert(graph->hasLessThenTwoEdgeWay("page6.html", "page1.html") == false);
    assert(graph->hasLessThenTwoEdgeWay("page6.html", "page2.html") == false);
    assert(graph->hasLessThenTwoEdgeWay("page6.html", "page3.html") == false);
    assert(graph->hasLessThenTwoEdgeWay("page6.html", "page4.html") == false);
    assert(graph->hasLessThenTwoEdgeWay("page6.html", "page5.html") == false);
    assert(graph->hasLessThenTwoEdgeWay("page6.html", "page7.html") == false);
    
    assert(graph->hasLessThenTwoEdgeWay("page7.html", "page1.html") == true);
    assert(graph->hasLessThenTwoEdgeWay("page7.html", "page2.html") == true);
    assert(graph->hasLessThenTwoEdgeWay("page7.html", "page3.html") == true);
    assert(graph->hasLessThenTwoEdgeWay("page7.html", "page4.html") == true);
    assert(graph->hasLessThenTwoEdgeWay("page7.html", "page5.html") == true);
    assert(graph->hasLessThenTwoEdgeWay("page7.html", "page6.html") == false);
}
