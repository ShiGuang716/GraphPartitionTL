
#include <iostream>
#include "graph.h"
#include "tl_graph.h"
#include <string>


int main(int argc, char *argv[]){
    #ifdef DEBUG
    initLogFile();
    #endif

    std::string filename;
    int nodeNumber = 6;             //默认节点数为6
    double perCalcCost = 2.0, perMesCost = 1.0;     //默认计算代价为2.0，通信代价为1.0

    /**
     * 读取命令行参数
     * 标准运行格式：
     * ./main filename [-n nodeNumber] [-m perMesCost] [-c perCalcCost]
     * filename 必须是第二个参数
     */
    try{
        if(argc<2){
            throw std::invalid_argument("Insufficient arguments");
        }

        filename = argv[1];

        for(int i=2; i < argc; ++i) {
            if(std::string(argv[i]) == "-n"){
                if(i+1 < argc) {
                    nodeNumber = std::stoi(argv[i+1]);
                    ++i;
                }
                else  throw std::invalid_argument("-n option requires an argument");
            }
            else if(std::string(argv[i]) == "-m"){
                if(i+1 < argc) {
                    perMesCost = std::stod(argv[i+1]);
                    ++i;
                }
                else  throw std::invalid_argument("-m option requires an argument");
            }
            else if(std::string(argv[i]) == "-c"){
                if(i+1 < argc) {
                    perCalcCost = std::stod(argv[i+1]);
                    ++i;
                }
                else  throw std::invalid_argument("-c option requires an argument");
            }
            else  throw std::invalid_argument("unkonwn option: "+ std::string(argv[i]));
        }
    }
    catch(const std::exception& e){
        std::cerr << "Error: "<<e.what() <<std::endl;
        return 1;
    }

    Log("perCalcCost = %lf,perMesCost = %lf,nodeNumber = %d", perCalcCost, perMesCost, nodeNumber);

    PartitionAlgorithm parAlgo(perCalcCost, perMesCost, nodeNumber);
    parAlgo.init(filename);

    MAP reassign = parAlgo.costSentitiveReassign();

    FILE* outputFile = fopen("output.txt", "w");
    if(!outputFile)     perror("Failed to open output file");
    for(auto it = reassign.begin(); it!=reassign.end(); ++it){
        fprintf(outputFile, "%d %d\n", it->first, it->second);
    }
    fclose(outputFile);

    #ifdef DEBUG
    closeLogFile();
    #endif

    return 0;
}