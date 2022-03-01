#include <entry.h>
#include "game.h"

b8 create_game(game* out_game) {
	out_game->app_config.name = "Kohi Engine";
	out_game->app_config.start_pos_x = 100;
	out_game->app_config.start_pos_y = 100;
	out_game->app_config.start_width = 1280;
	out_game->app_config.start_height = 720;
	out_game->update = game_update;
	out_game->render = game_render;
	out_game->initialize = game_initialize;
	out_game->on_resize = game_on_resize;

	out_game->state_memory_requirement = sizeof(game_state);
	out_game->application_state = 0;
	out_game->state = 0;
	return true;
}