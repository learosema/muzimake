#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

static FILE * log_fd = nullptr;

void log_open()
{
	log_fd = fopen("muzimake.log", "a");
}

void log_close()
{
	fclose(log_fd);
	log_fd = nullptr;
}


void log_write(const char* fmt, ...)
{
	char datetime_str[64];
	va_list args;
	time_t t = time(NULL);
	struct tm * tm = localtime(&t);

	va_start(args, fmt);
	if (log_fd == nullptr) {
		log_open();
		atexit(log_close);
	}

	strftime(datetime_str, sizeof(datetime_str), "[%Y-%m-%d - %H:%M:%S] ", tm);
	fprintf(log_fd, datetime_str);
	vfprintf(log_fd, fmt, args);
	fprintf(log_fd, "\n");
	va_end(args);
}

