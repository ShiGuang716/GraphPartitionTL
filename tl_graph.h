
#ifndef tl_graphH
#define tl_graphH

#include "graph.h"
#include <unordered_set>
#include <unordered_map>

struct Modification {
    double cost;
    std::unordered_map<int, int> map;
};

double computeCost(GRAPH& graph, std::unordered_map<int, int> reassign);
std::unordered_map<int, int> createReassignment(std::unordered_set<int> failedPartitions, std::unordered_set<int> nodes);

Modification nextChange(GRAPH &graph, 
    std::unordered_set<int> failedPartitions, 
    std::unordered_set<int> nodes,
    std::unordered_map<int, int> reassign);

void CostSentitiveReassign(GRAPH &graph, 
    std::unordered_set<int> failedPartitions, 
    std::unordered_set<int> nodes);

#endif