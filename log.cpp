#include"log.h"
void logMessageFromCpp(const char *message) {
    qDebug() << "C++ Log:" << message;
}

void logMessageFromCppFormat(const char *__format, ...) {
    char logbuf[256];  // Larger buffer to accommodate formatted strings
    memset(logbuf, 0, sizeof(logbuf));

    // Start variadic argument processing
    va_list args;
    va_start(args, __format);

    // Use vsnprintf for safety (to avoid buffer overflow)
    vsnprintf(logbuf, sizeof(logbuf), __format, args);

    // End variadic argument processing
    va_end(args);

    // Log the formatted message using qDebug and logMessageFromCpp
    qDebug() << "game Log:" << logbuf;

    // Optionally, call another logging function (if needed)
    // logMessageFromCpp(logbuf);
}
