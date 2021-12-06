#pragma once

#include "defines.h"

#ifdef _DEBUG
#define LOG_MESSAGE_LENGTH 1000

#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1



enum log_level {
	LOG_LEVEL_FATAL,
	LOG_LEVEL_ERROR, 
	LOG_LEVEL_WARN,
	LOG_LEVEL_INFO,
	LOG_LEVEL_DEBUG,
	LOG_LEVEL_TRACE	
};

b8 initialize_logging(u64* memory_requirement, void* state);
void shutdown_logging(void* state);
KAPI void log_output(enum log_level level, const char* messageformat, ...);

#ifndef EN_FATAL
#define EN_FATAL(x, ...)	log_output(LOG_LEVEL_FATAL, x, ##__VA_ARGS__)	
#endif

#ifndef EN_ERROR
#define EN_ERROR(x, ...)	log_output(LOG_LEVEL_ERROR, x, ##__VA_ARGS__)	
#endif

#if LOG_WARN_ENABLED
#define EN_WARN(x, ...)	log_output(LOG_LEVEL_WARN, x, ##__VA_ARGS__)	
#else 
#define EN_WARN(x, ...)
#endif

#if LOG_DEBUG_ENABLED
#define EN_DEBUG(x, ...)	log_output(LOG_LEVEL_DEBUG, x, ##__VA_ARGS__)	
#else 
#define EN_DEBUG(x, ...)
#endif


#if LOG_INFO_ENABLED
#define EN_INFO(x, ...)	log_output(LOG_LEVEL_INFO, x, ##__VA_ARGS__)	
#else 
#define EN_INFO(x, ...)
#endif

#if LOG_TRACE_ENABLED
#define EN_TRACE(x, ...)	log_output(LOG_LEVEL_TRACE, x, ##__VA_ARGS__)	
#else 
#define EN_TRACE(x, ...)
#endif

#endif
