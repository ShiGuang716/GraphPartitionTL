
/*------------------------------------------------------------*/
#include "graph.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
/*------------------------------------------------------------*/

void GRAPH::read(char* filename){
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

    this->V1 = this->partitions.size();
    this->initPar2node = std::vector<int>(this->V1);
    //初始化partitionId到nodeId的映射
    for(int i=0; i<V1; ++i) {
        initPar2node[i] = Partition::getInitNodeId(i);
    }

    //保存失效分区Id到变量failedPartitions中
    for(const auto& P : this->partitions) {
        if(Partition::isFailedPartition(P.Id)) {
            this->failedPartitions.insert(P.Id);
        }
    }

    //默认所有计算结点都是可分配计算结点
    for(int i=0; i<V1; ++i) {
        this->nodes.insert(initPar2node[i]);
    }

    //输出读取的图结构
    // for(const auto& partition : partitions) {
    //     std::cout<<partition.Id<<" "<<partition.calcTimes<<"   ";

    //     for(const auto& edge: partition.edges){
    //         std::cout<<edge.targetPartitionId<<" "<<edge.sendMessageTimes<<"  ";
    //     }
    //     std::cout<<std::endl;
    // }
}

double GRAPH::getCost(){
    int totalMesTimes = 0, totalCalcTimes = 0;
    std::vector<int> perNodeCalcTimes(nodeNumber);  //各个node上的总计算代价
    //遍历所有分区
    for(const auto & partition : this->partitions) {
        //如果该partition是失效partition，将其计算成本计入代价中
        if(Partition::isFailedPartition(partition.Id)){
            perNodeCalcTimes[par2node[partition.Id]] += partition.calcTimes;
        }

        //若src和dest在同一node上，通信次数为0。否则：
        //若src为未失效分区，dest为未失效分区，通信次数为0
        //若src为未失效分区，dest为失效分区，通信次数为边权值
        //若src为失效分区，dest为未失效分区，通信次数为1
        //若src为失效分区，dest为失效分区，通信次数为边权值

        bool isSrcFailed = Partition::isFailedPartition(partition.Id);
        bool isDestFailed;
        for(const auto& edge: partition.edges) {
            //如果两个分区在同一个node上，通信次数为0
            //TODO()    如果partitionId和partitions数组的下标不对应，这里需要修改！！
            if(par2node[partition.Id] == par2node[partitions[edge.targetPartitionId].Id])
                continue;
            
            isDestFailed = Partition::isFailedPartition(edge.targetPartitionId);
            
            if(isDestFailed){
                totalMesTimes += edge.sendMessageTimes;
            }
            else if(isSrcFailed){
                totalMesTimes += 1;
            }

        }
    }

    //获取各个node计算次数的最大值，作为总计算次数
    totalCalcTimes = *std::max_element(perNodeCalcTimes.begin(), perNodeCalcTimes.end());

    //std::cout << perCalcCost*totalCalcTimes << "+" << perMesCost*totalMesTimes<<std::endl;

    double totalCost = perMesCost*totalMesTimes + perCalcCost*totalCalcTimes;

    return totalCost;
}

double seconds() {
    return 0;
}
