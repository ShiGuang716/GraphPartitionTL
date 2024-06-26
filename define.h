#ifndef DEFINE_H_
#define DEFINE_H_

#include <cstdio> // 包含printf和fprintf的头文件
#include <cstdarg> // 包含va_list和相关函数的头文件

#define MAP std::unordered_map<int, int>
#define SET std::unordered_set<int>

//初始化日志文件
void initLogFile();

//关闭日志文件
void closeLogFile();

// 实现日志输出到文件的函数
void logToFile(const char* file, const char* function, int line, const char* format, ...) ;

/**
 * 获取分区初始所在节点的Id
 */
int getInitNodeId(int partitionId, int nodeNumber);

/**
 * 判断分区是否为失效分区
 */
bool isFailed(int partitionId, int nodeNumber);

#ifdef DEBUG
#define Log(format, ...) logToFile(__FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)
#else
#define Log(format, ...)
#endif

#endif