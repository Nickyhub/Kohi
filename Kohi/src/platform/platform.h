#pragma once

#include "defines.h"

b8 platform_system_startup(
	u64* memory_requirement,
	void* state,
	const char* application_name,
	i32 x,
	i32 y,
	i32 width,
	i32 height);

void platform_system_shutdown(void* plat_state);
b8 platform_pump_messages();

void* platform_allocate(unsigned long long size, b8 aligned);
void  platform_free(void* block, b8 aligned);
void* platform_zero_memory(void* block, unsigned long long size);
void* platform_copy_memory(void* dest, const void* source, unsigned long long size);
void* platform_set_memory(void* dest, int value, unsigned long long size);

void platform_console_write(const char* message, char colour);
void platform_console_write_error(const char* message, char colour);

double platform_get_absolute_time();
void platform_sleep(unsigned long long ms);