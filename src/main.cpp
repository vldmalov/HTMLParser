#include <iostream>
#include <vector>
#include <cassert>

#include "XHTMLParser.h"
#include "FileHelper.h"
#include "DirectedGraph.h"

void showUsageHint();
void checkGraphData(std::shared_ptr<DirectedGraph> graph);

int main(int argc, const char * argv[]) {
    
    if(argc != 2) {
        showUsageHint();
        exit(1);
    }
    
    std::string dirName(argv[1]);
    std::vector<std::string> fileNames = FileHelper::getDirFileNames(dirName, "html");
    if(fileNames.empty()) {
        std::cout << "Files not found! For proccessing required at least one xHTML file" << std::endl;
        exit(1);
    }
    
    std::shared_ptr<DirectedGraph> graph(new DirectedGraph());
    std::unordered_map<std::string, std::shared_ptr<HTMLTag>> parsedXHTMLs;
    
    for(const std::string fileName : fileNames)
    {
        std::cout << "Process file : " << fileName << std::endl;
        
        std::string fullFileName = FileHelper::getFullFileName(dirName, fileName);
        std::shared_ptr<XHTMLParser> parser(new XHTMLParser(fullFileName));
        parser->process();
        
        std::shared_ptr<HTMLTag> rootTag = parser->getRootTag();
        parsedXHTMLs.insert(std::make_pair(fileName, rootTag));

        std::vector<std::string> pageLinks = parser->getPageLinks();
        for(const std::string& link : pageLinks) {
            graph->addEdge(fileName, link);
        }
    }
    
    // Check results
    
    // Print all parsed HTML file structures
    for(auto parsedXHTML : parsedXHTMLs) {
        std::cout << "Parsed XHTML name: " << parsedXHTML.first << std::endl;
        
        std::shared_ptr<HTMLTag> htmlRoot = parsedXHTML.second;
        std::cout << *htmlRoot.get() << std::endl;
    }

    // Check graphs: hasDirectEdge, hasLessThenTwoEdgeWay and serialize
    checkGraphData(graph);
    
    graph->saveToFile("graph.gr");
    
    std::shared_ptr<DirectedGraph> secondGraph(new DirectedGraph());
    secondGraph->loadFromFile("graph.gr");
    checkGraphData(secondGraph);
    
    return 0;
}

void showUsageHint()
{
    std::cout << "usage: htmlParser source_directory" << std::endl;
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
