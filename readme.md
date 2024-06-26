## 编译：
g++ -o main main.cpp graph.cpp tl_graph.cpp define.cpp
或者
g++ -o main main.cpp graph.cpp tl_graph.cpp define.cpp -D DEBUG
后者会启用日志记录

## 运行:
./main filename [-n nodeNumber] [-m perMesCost] [-c perCalcCost]
nodeNumber为节点数量，perMesCost为单次通信成本，perCalcCost为单次计算成本
filename必须作为第二个参数

## 输出：
重分区结果保存在output.txt文件中，一行表示一个失效分区的分配节点：
[failedPartitionID, reassignNodeID]

若编译时启用条件编译-D DEBUG，则会将日志输出到log.txt中
