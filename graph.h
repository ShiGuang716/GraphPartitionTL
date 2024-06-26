#ifndef graphH
#define graphH

/*---------------------------------------------------------------------*/
#include <vector>
#include <unordered_set>
#include <string>
#include "define.h"
/*---------------------------------------------------------------------*/

struct Edge{
    int targetPartitionId;//目标分区ID
    int sendMessageTimes ;//消息发送次数
};

struct Partition{
    int Id;//分区Id
    int calcTimes;      //分区的计算次数
    std::vector<Edge> edges;
};

class GRAPH {
public:
    int V;                              //顶点个数
    std::vector<Partition> partitions;  //顶点（分区）列表

public:
    GRAPH(){}

    /**
     * 读取文件
    */
    void read(std::string );

    /**
     * 获取分区结果下的总计算次数
     */
    int getCalcTimes(const std::vector<int>& par2node, int nodeNumber);

    /**
     * 获取分区结果下的总通信次数
     */
    int getMesTimes(const std::vector<int>& par2node, int nodeNumber);

    /**
     * 输出图结构
     */
    void printGraph();

};


#endif