#include "core/kstring.h"
#include "core/kmemory.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

u64 string_length(const char* str) {
	return strlen(str);
}

char* string_duplicate(const char* str) {
	u64 length = string_length(str);
	char* copy = kallocate(length + 1, MEMORY_TAG_STRING);
	kcopy_memory(copy, str, length + 1);
	return copy;
}

// Case-sensitive string comparison. true if the same, otherwise false.
b8 strings_equal(const char* str0, const char* str1) {
	return strcmp(str0, str1) == 0;
}

i32 string_format(char* dest, const char* format, ...) {
	if (dest) {
		va_list arg_ptr = 0;
		va_start(arg_ptr, format);
		i32 written = string_format_v(dest, format, arg_ptr);
		va_end(arg_ptr);
		return written;
	}
	return -1;
}

i32 string_format_v(char* dest, const char* format, void* va_list) {
	if (dest) {
		char buffer[16000];
		i32 written = vsnprintf(buffer, 32000, format, va_list);
		buffer[written] = 0;
		kcopy_memory(dest, buffer, written + 1);
		return written;
	}
	return -1;
}