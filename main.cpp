
#include <iostream>
#include "graph.h"
#include "tl_graph.h"

int main(){
    char filename[80];
    int nodeNumber;

    std::cout<<"Please enter filename:"<<std::endl;
    std::cin>>filename;

    std::cout<<"Please enter the number of nodes:"<<std::endl;
    std::cin>>nodeNumber;

    try{
        double time = seconds();
        GRAPH graph(2.0, 1.0, nodeNumber);
        graph.read(filename);

        CostSentitiveReassign(graph, graph.failedPartitions, graph.nodes);

    }catch(...){}

    return 1;
}