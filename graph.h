#ifndef graphH
#define graphH

/*---------------------------------------------------------------------*/
#include <vector>
#include <unordered_set>
/*---------------------------------------------------------------------*/

const int nodeNumber = 3;       //后面要改成通过命令行/文件输入的形式
const int failedNodeId = 2;

struct Edge{
    int targetPartitionId;//目标分区ID
    int sendMessageTimes ;//消息发送次数
};

struct Partition{
    int Id;//分区Id
    int calcTimes;      //分区的计算次数
    std::vector<Edge> edges;

    /**
     * 通过partitionId获取初始nodeId
     * 静态成员函数，方便在没有实例化对象之前调用
    */
    static int getInitNodeId(int partitionId) {
        return partitionId % nodeNumber;
    }

    /**
     * 判断partition是否是失效partition
    */
    static bool isFailedPartition(int partitionId) {
        return partitionId % nodeNumber == failedNodeId;
    }
};

class GRAPH {
public:
    int V1;        //顶点个数
    std::vector<Partition> partitions;  //顶点（分区）列表

    double perMesCost, perCalcCost; //单次通信成本和单次计算成本

    std::vector<int> par2node;      //partitionId到nodeId的映射
    std::vector<int> initPar2node;  //初始partitionId到nodeId的映射

    std::unordered_set<int> failedPartitions;   //失效分区
    std::unordered_set<int> nodes;              //可分配计算结点

public:
    GRAPH(double c1=1.0, double c2=1.0) {
        perCalcCost = c1, perMesCost = c2;
    }

    /**
     * 读取文件
    */
    void read(char *);

    /**
     * 返回当前解的代价
    */
    double getCost();

};

/**
 * 返回当前时间
*/
double seconds();


#endif