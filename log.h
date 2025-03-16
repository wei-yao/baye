// log.h
#ifndef LOG_H
#define LOG_H

#include<QDebug>
#include<cstring>

#ifdef __cplusplus
extern "C" {
#endif

void logMessageFromCpp(const char *message);
void logMessageFromCppFormat(const char *__format, ...);

#ifdef __cplusplus
}
#endif

#endif // LOG_H