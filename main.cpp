
#include <iostream>
#include "graph.h"
#include "tl_graph.h"

int main(){
    char filename[80];

    std::cout<<"Please enter filename:"<<std::endl;
    std::cin>>filename;

    try{
        double time = seconds();
        GRAPH graph(1.0, 1.0);
        graph.read(filename);

        CostSentitiveReassign(graph, graph.failedPartitions, graph.nodes);

    }catch(...){}

    return 1;
}