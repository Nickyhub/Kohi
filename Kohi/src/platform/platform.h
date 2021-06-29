#pragma once

#include "defines.h"

typedef struct platform_state{
	void* internal_state;
} platform_state;

b8 platform_startup(platform_state* plat_state, const char* app_name, int x, int y, int width, int height);
void platform_shutdown(platform_state* plat_state);
b8 platform_pump_messages(platform_state* plat_state);

void* platform_allocate(unsigned long long size, b8 aligned);
void  platform_free(void* block, b8 aligned);
void* platform_zero_memory(void* block, unsigned long long size);
void* platform_copy_memory(void* dest, const void* source, unsigned long long size);
void* platform_set_memory(void* dest, int value, unsigned long long size);

void platform_console_write(const char* message, char colour);
void platform_console_write_error(const char* message, char colour);

double platform_get_absolute_time();
void platform_sleep(unsigned long long ms);