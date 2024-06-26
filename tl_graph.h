
#ifndef tl_graphH
#define tl_graphH

#include "graph.h"
#include <unordered_set>
#include <unordered_map>

/**
 * 记录重分区结果
 */
struct Modification {
    double cost;                        //重分区后的代价
    int calcTimes, mesTimes;            //重分区后的计算次数和通信次数
    std::unordered_map<int, int> exchange;   //重分区映射 map<P, N> 表示将分区P重新分配到节点N上

    Modification(){}

};

class PartitionAlgorithm{
    public:
    std::unordered_set<int> failedPartitions;   //失效分区
    std::unordered_set<int> nodes;              //可分配的计算节点
    double perCalcCost, perMesCost;             //单次计算成本和单次通信成本

    GRAPH graph;                                //保存分区间关系的图结构
    int nodeNumber;                             //计算节点数

    public:
    PartitionAlgorithm(double calcCost, double mesCost, int nodeNum):
        nodeNumber(nodeNum), perCalcCost(calcCost), perMesCost(mesCost){}

    /**
     * 为重分区所需的数据进行初始赋值
     */
    void init(const std::string& filename);

    /**
    * 计算重分区代价
    * 函数中用到：graph
    * 输入：
    * reassign:当前重分区
    */
    double computeCost(MAP& reassign);

    /**
    * 创建初始重分区
    * 函数中用到：failedPartitions, nodes
    */
    std::unordered_map<int, int> createReassignment();

    /**
     * 根据当前重分区，获取下一个重分区结果
     * 函数中用到：graph, nodes
     * 输入：
     * reassign:当前重分区， partitions:还未作为交换主体的分区
     */
    Modification nextChange(std::unordered_map<int, int>&, std::unordered_set<int>& );

    /**
     * 获取最优重分区
     * 函数中用到：graph, failedPartitions, nodes
     */
    MAP costSentitiveReassign();

};

#endif