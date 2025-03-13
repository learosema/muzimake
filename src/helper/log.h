#ifndef __LOG_H__
#define __LOG_H__

#ifdef DEBUG
#define APP_LOG(...) log_write(__VA_ARGS__)
#else
#define APP_LOG(...)
#endif


void log_write(const char* fmt, ...);

#endif