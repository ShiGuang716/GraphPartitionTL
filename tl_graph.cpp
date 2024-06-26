
/*-----------------------------------------------------------*/
#include "tl_graph.h"
#include <limits>
#include <random>
#include <iostream>
#include <algorithm>

/*-----------------------------------------------------------*/

void PartitionAlgorithm::init(const std::string& filename){
    //读取图结构
    graph.read(filename); 

    //保存失效分区Id到failedPartitions中
    for(const auto& P:graph.partitions) {
        if(isFailed(P.Id, nodeNumber)){
            failedPartitions.insert(P.Id);
        }
    }

    //所有计算节点都是可分配计算节点
    for(int i=0; i<nodeNumber; ++i) {
        nodes.insert(i);
    }
}

double PartitionAlgorithm::computeCost(MAP& reassign) {
    //获取各个分区的分配结果，保存在par2node数组中
    std::vector<int> par2node(graph.V);
    for(int i=0; i<graph.V; ++i){
        par2node[i] = getInitNodeId(i, nodeNumber);
    }
    for(auto it=reassign.begin(); it!=reassign.end(); ++it) {
        par2node[it->first] = it->second;
    }

    //获取计算次数
    int totalCalcTimes = graph.getCalcTimes(par2node, nodeNumber);
    //获取通信次数
    int totalMesTimes = graph.getMesTimes(par2node, nodeNumber);
    //获取总代价
    double totalCost = perMesCost*totalMesTimes + perCalcCost*totalCalcTimes;

    Log("计算次数为 %d", totalCalcTimes);
    Log("通信次数为 %d", totalMesTimes);
    Log("总代价为 %lf", totalCost);

    return totalCost;
}

std::unordered_map<int, int> PartitionAlgorithm::createReassignment() {
    std::unordered_map<int, int> reassign;
    std::vector<int> nodesArray(nodes.begin(), nodes.end());

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, nodes.size()-1);

    for(int P : failedPartitions) {
        int randomIdx = dis(gen);
        int randomNode = nodesArray[randomIdx];

        reassign[P] = randomNode;
    }

    return reassign;
}

Modification PartitionAlgorithm::nextChange(MAP & reassign, SET & partitions) {
    Modification L;
    L.cost = 1e9;

    //选择一个分区作为交换主体
    //将作为交换主体的分区与另一个分区交换位置，
    //或者将作为交换主体的分区移动到另一个节点
    //遍历所有可能，选择其中总代价最小的方案
    for(int P : partitions) {
        for(int _P : partitions) {
            if(P == _P || reassign[P] == reassign[_P])      continue;

            std::unordered_map<int, int> _reassign(reassign);
            _reassign[P] = reassign[_P];
            _reassign[_P] = reassign[P];

            double t = computeCost(_reassign);

            if(L.cost > t) {
                L.exchange.clear();
                L.exchange[P] = reassign[_P];
                L.exchange[_P] = reassign[P];
                L.cost = t;
            }
        }

        for(int N : nodes) {
            if(N == reassign[P])    continue;

            std::unordered_map<int, int> _reassign(reassign);
            _reassign[P] = N;

            double t = computeCost(_reassign);

            if(L.cost > t) {
                L.exchange.clear();
                L.exchange[P] = N;
                L.cost = t;
            }
        }
    }

    Log("L.cost = %lf", L.cost);
    return L;
}

MAP PartitionAlgorithm::costSentitiveReassign() {
    //随机生成一个初始解并计算其代价
    std::unordered_map<int, int> reassign = createReassignment();
    double Tlow = computeCost(reassign);

    Log("初始Tlow为 %lf", Tlow);

    //Ls保存每一步的微小变化和变化后的代价
    std::vector<Modification> Ls;

    while(true) {
        std::unordered_map<int, int> _reassign(reassign);
        std::unordered_set<int> _partitions(failedPartitions);

        //当所有分区都作为一次交换主体后退出循环
        while(!_partitions.empty()) {
            Modification L = nextChange(_reassign, _partitions);

            for(auto it = L.exchange.begin(); it!=L.exchange.end(); ++it) {
                _reassign[it->first] = it->second;
                _partitions.erase(it->first);
            }

            Ls.push_back(L);
        }

        //查找在sizeof(failedPartitions)次微小操作中代价最小的一次，下标idx
        double t = std::numeric_limits<double>::infinity();
        int idx = -1;
        for(int i=0; i<Ls.size(); ++i) {
            if(Ls[i].cost < t) {
                t = Ls[i].cost;
                idx = i;
            }
        }

        //如果该轮操作得到的代价比之前记录的更小，则更新代价和重分区，并继续迭代
        //否则结束程序
        if(Ls[idx].cost < Tlow) {
            for(int j=0; j<=idx; ++j) {
                for(auto it = Ls[j].exchange.begin(); it!=Ls[j].exchange.end(); ++it) {
                    reassign[it->first] = it->second;
                }
            }
            Tlow = Ls[idx].cost;
            Log("Tlow 更新为 %lf", Tlow);
        }
        else{
            printf("reassign partitions result:\n");
            for(auto it = reassign.begin(); it!=reassign.end(); ++it) {
                printf("%d---->%d\n", it->first, it->second);
            }
            printf("Tlow = %lf\n", Tlow);
            break;
        }

    }

    return reassign;
}
