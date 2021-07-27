#include "logger.h"

#ifdef _DEBUG

#define EN_ASSERT_MSG(expression, message) if(!expression) { log_output(0, "Assertion failed! %s Line: %i %s", __FILE__, __LINE__, message); __debugbreak(); }
#define EN_ASSERT(expression) if(!expression) { log_output(0, "Assertion failed! %s Line: %i", __FILE__, __LINE__); __debugbreak(); }
#endif