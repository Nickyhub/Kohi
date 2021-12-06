#pragma once

#include "defines.h"


// Returns the length of the given string.
KAPI u64 string_length(const char* str);

KAPI char* string_duplicate(const char* str);

// Case-sensitive string comparison. true if the same, otherwise false.
KAPI b8 strings_equal(const char* str0, const char* str1);

KAPI i32 string_format(char* dest, const char* format, ...);

KAPI i32 string_format_v(char* dest, const char* format, void* va_list);