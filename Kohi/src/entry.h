#pragma once

#include "core/logger.h"
#include "gametypes.h"
#include "core/application.h"
#include "core/kmemory.h"

extern b8 create_game(game* out_game);

int main() {
	initialize_memory();

	game game_inst;
	if (!create_game(&game_inst)) {
		EN_FATAL("Could not create game");
		return -1;
	}

	if(!game_inst.render || !game_inst.update || !game_inst.initialize || !game_inst.on_resize) {
		EN_FATAL("The game's function pointers must be assigned!");
		return -2;
	}

	if (!application_create(&game_inst)) {
		EN_INFO("Application failed to create!");
		return 1;
	}
	if (!application_run()) {
		EN_INFO("Application did not shutdown gracefully");
		return 2;
	}
	shutdown_memory();

	return 0;
}