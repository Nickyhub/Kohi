#pragma once
#include <core/logger.h>
//#include <math/kmath.h>

#define expect_should_be(expected, actual)																\
if (actual != expected) {																				\
	EN_ERROR("--> Expected %lld, but got: %lld. File %s:%d.", expected, actual, __FILE__, __LINE__);	\
	return false;																						\
}

#define expect_should_not_be(expected, actual)																	\
if (actual == expected) {																						\
	EN_ERROR("--> Expected %d != %d, but they are eqal. File: %s:%d", expected, actual, __FILE__, __LINE__);	\
	return false;																								\
}

#define expect_float_to_be(expected, actual)														\
if (kabs(expected - actual) > 0.001f) {																\
	EN_ERROR("--> Expected %f, but got: %f. File %s:%d.", expected, actual, __FILE__, __LINE__);	\
	return false;																					\
}															

#define expect_to_be_true(actual)													\
if (actual != true) {																\
	EN_ERROR("--> Expected true, but got false. File: %s:%d", __FILE__, __LINE__);	\
	return false;																	\
}

#define expect_to_be_false(actual)													\
if (actual != false) {																\
	EN_ERROR("--> Expected false, but got true. File: %s:%d", __FILE__, __LINE__);	\
	return false;																	\
}