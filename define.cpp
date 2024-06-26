
#include "define.h"

FILE* logFile = nullptr;

void initLogFile() {
    logFile = fopen("log.txt", "w");
    if(!logFile){
        perror("Failed to open log file");
    }
}

void closeLogFile(){
    if(logFile) {
        fclose(logFile);
    }
}

void logToFile(const char* file, const char* function, int line, const char* format, ...) {
    if (!logFile) return; // 如果日志文件未打开，则返回

    // 写入文件名、函数名和行号
    fprintf(logFile, "[%s][%s][%d]: ", file, function, line);

    // 处理可变参数列表
    va_list args;
    va_start(args, format);
    vfprintf(logFile, format, args);
    va_end(args);

    // 写入换行符
    fprintf(logFile, "\n");

    // 刷新文件缓冲区
    fflush(logFile);
}

int getInitNodeId(int partitionId, int nodeNumber) {
    //分区Id对节点数取模，得到分区所在节点Id
    return partitionId % nodeNumber;
}

bool isFailed(int partitionId, int nodeNumber) {
    //初始第0个节点上的分区为失效分区
    return getInitNodeId(partitionId, nodeNumber) == 0;
}