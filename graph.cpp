
/*------------------------------------------------------------*/
#include "graph.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
/*------------------------------------------------------------*/

/**
 * 文件构成：
 * [partitionID,calcTimes,[[targetPartitionID,messageTimes],[targetPartitionID,messageTimes]...]]
 * 
 * 例：
 * [0,1,[[1,1],[3,3]]]
 * [1,0,[[0,1],[2,2],[3,1]]]
 */
void GRAPH::read(std::string filename){
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)){
        std::istringstream iss(line);

        char ch;
        int partitionId, calcTimes;
        std::vector<Edge> edges;

        // 读取 partitionId 和 calcTimes
        iss >> ch >> partitionId >> ch >> calcTimes >> ch;

        // 读取每个子字符串 "[x,y]"
        std::string token;
        while (std::getline(iss, line, '[')){
            if(line.empty())    continue;
            std::istringstream edgeIss(line);
            int targetPartitionId, sendMessageTimes;
            char comma;
            edgeIss >> targetPartitionId >> comma >> sendMessageTimes;
            edges.push_back({targetPartitionId, sendMessageTimes});
        }

        // 将 partition 添加到 graph.partitions 中
        this->partitions.push_back({partitionId, calcTimes, edges});
    }

    //printGraph();
}

int GRAPH::getCalcTimes(const std::vector<int>& par2node, int nodeNumber) {
    int totalCalcTimes = 0;
    std::vector<int> perNodeCalcTimes(nodeNumber);      //各个节点上的总计算次数

    for(const auto & partition: partitions) {
        if(isFailed(partition.Id, nodeNumber)){
            int nodeId = par2node[partition.Id];
            perNodeCalcTimes[nodeId] += partition.calcTimes;
        }
    }
    totalCalcTimes = *std::max_element(perNodeCalcTimes.begin(), perNodeCalcTimes.end());

    return totalCalcTimes;
}

int GRAPH::getMesTimes(const std::vector<int>& par2node, int nodeNumber) {
    int totalMesTimes = 0;

    for(const auto& P : partitions) {
        for(const auto& edge : P.edges) {
            //如果两个分区在同一个node上，通信次数为0
            int srcNode = par2node[P.Id];
            int destNode = par2node[edge.targetPartitionId];
            if(srcNode == destNode) continue;

            //若dest为未失效分区，通信次数为0
            //若dest为失效分区，通信次数为边的权值
            if(isFailed(edge.targetPartitionId, nodeNumber)) {
                totalMesTimes += edge.sendMessageTimes;
            }
        }
    }

    return totalMesTimes;
}

void GRAPH::printGraph(){
    //输出读取的图结构
    for(const auto& partition : partitions) {
        printf("[%d,%d,[", partition.Id, partition.calcTimes);

        for(const auto& edge: partition.edges){
            printf("[%d,%d],", edge.targetPartitionId, edge.sendMessageTimes);
        }

        printf("]]\n");
    }
}

double seconds() {
    return 0;
}
