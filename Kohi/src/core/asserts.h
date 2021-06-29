#include "logger.h"

#ifdef _DEBUG

#define EN_ASSERT(expression, message) if(!expression) { log_output(0, "Assertion failed! %s Line: %i %s", __FILE__, __LINE__, message); __debugbreak(); }




#endif