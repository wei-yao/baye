// log.h
#ifndef LOG_H
#define LOG_H

#include<QDebug>
#include<cstring>

void logMessageFromCpp(const char *message);
void logMessageFromCppFormat(const char *__format, ...);

#endif // LOG_H