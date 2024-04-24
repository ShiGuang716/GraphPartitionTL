
/*-----------------------------------------------------------*/

#include "tl_graph.h"
#include <limits>
#include <random>
#include <iostream>
/*-----------------------------------------------------------*/

double computeCost(GRAPH& graph, std::unordered_map<int, int> reassign) {
    graph.par2node = std::vector<int>(graph.initPar2node);

    for(auto it=reassign.begin(); it!=reassign.end(); ++it) {
        graph.par2node[it->first] = it->second;
    }

    return graph.getCost();
}


std::unordered_map<int, int> createReassignment(std::unordered_set<int> failedPartitions, std::unordered_set<int> nodes) {
    std::unordered_map<int, int> reassign;
    // std::vector<int> nodesArray(nodes.begin(), nodes.end());

    // std::random_device rd;
    // std::mt19937 gen(rd());
    // std::uniform_int_distribution<> dis(0, nodes.size()-1);

    // for(int P : failedPartitions) {
    //     int randomIdx = dis(gen);
    //     int randomNode = nodesArray[randomIdx];

    //     reassign[P] = randomNode;
    // }

    reassign[0] = 1;
    reassign[1] = 0;
    reassign[2] = 0;

    return reassign;
}

Modification nextChange(GRAPH &graph, 
    std::unordered_set<int> failedPartitions, 
    std::unordered_set<int> nodes,
    std::unordered_map<int, int> reassign) {
    
    Modification L;
    L.cost = 1e9;

    for(int P : failedPartitions) {
        for(int _P : failedPartitions) {
            if(P == _P || reassign[P]== reassign[_P])     continue;
            std::unordered_map<int, int> _reassign(reassign);
            _reassign[P] = reassign[_P];
            _reassign[_P] = reassign[P];

            double t = computeCost(graph, _reassign);

            if(L.cost > t) {
                L.map.clear();
                L.map[P] = reassign[_P];
                L.map[_P] = reassign[P];
                L.cost = t;
            }

        }

        for(int N :nodes) {
            if(N == reassign[P])        continue;
            std::unordered_map<int, int> _reassign(reassign);
            _reassign[P] = N;

            double t = computeCost(graph, _reassign);
        
            if(L.cost > t) {
                L.map.clear();
                L.map[P] = N;
                L.cost = t;
            }

        }

    }

    return L;
}

void CostSentitiveReassign(GRAPH &graph, 
    std::unordered_set<int> failedPartitions, 
    std::unordered_set<int> nodes) {
    
    std::unordered_map<int, int> reassign = createReassignment(failedPartitions, nodes);
    double Tlow = computeCost(graph, reassign);

    //输出初始化结果
    for(auto it = reassign.begin(); it != reassign.end(); ++it) {
        std::cout << (it->first) << "---->" << (it->second) << std::endl;
    }
    std::cout << "Tlow = " << Tlow << std::endl;

    std::vector<Modification> Ls;

    while(true) {
        std::unordered_map<int, int> _reassign(reassign);
        std::unordered_set<int> _partitions(failedPartitions);

        while(!_partitions.empty()) {
            Modification L = nextChange(graph, _partitions, nodes, _reassign);

            for(auto it = L.map.begin(); it != L.map.end(); ++it) {
                _reassign[it->first] = it->second;
                _partitions.erase(it->first);
            }

            Ls.push_back(L);
        }

        double t = std::numeric_limits<double>::infinity();
        int idx = -1;
        for(int i = 0; i<Ls.size(); ++i) {
            if(Ls[i].cost < t) {
                t = Ls[i].cost;
                idx = i;
            }
        }

        if(Ls[idx].cost < Tlow) {
            for(int j=0; j<=idx; ++j) {
                for(auto it = Ls[j].map.begin(); it != Ls[j].map.end(); ++it) {
                    reassign[it->first] = it->second;
                }
            }

            Tlow = Ls[idx].cost;

            //输出更新结果
            for(auto it = reassign.begin(); it != reassign.end(); ++it) {
                std::cout << (it->first) << "---->" << (it->second) << std::endl;
            }
            std::cout << "Tlow = " << Tlow << std::endl;
        }
        else{
            break;
        }

    }
    
    std::cout << "Tlow = " << Tlow << std::endl;

    return ;
}