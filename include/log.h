#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <pthread.h>

typedef enum
{
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
} LogLevel;

void log_init(const char *filename);

void log_message(LogLevel level, const char *format, ...);

void log_cleanup();

#endif // LOG_H
