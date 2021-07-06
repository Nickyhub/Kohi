#pragma once
#include "defines.h"
struct game;

typedef struct application_config {
	i16 start_pos_x;
	i16 start_pos_y;
	i16 start_width;
	i16 start_height;

	const char* name;
} application_config;

b8 KAPI application_create(struct game* game_inst);
b8 KAPI application_run();