#ifndef __LOGGING_H
#define __LOGGING_H
#include <stdio.h>
#include <pthread.h>

#define LOG(...) { \
	pthread_mutex_lock(&log_mutex); \
	fprintf(stdout, __VA_ARGS__); \
	pthread_mutex_unlock(&log_mutex); \
}

#define LOG_ERR(...) { \
	pthread_mutex_lock(&log_mutex); \
	fprintf(stderr, __VA_ARGS__); \
	pthread_mutex_unlock(&log_mutex); \
}

#if 0
#define LOG_VERBOSE(...) LOG(__VA_ARGS__)
#else
#define LOG_VERBOSE(...)
#endif

#ifdef __cplusplus
extern "C"{
#endif

extern pthread_mutex_t log_mutex;
void init_logging();

#ifdef __cplusplus
}
#endif

#endif //__LOGGING_H
