#include "application.h"
#include "platform/platform.h"
#include "logger.h"
#include "gametypes.h"
#include "kmemory.h"
#include "core/event.h"

typedef struct application_state {
	game* game_inst;
	b8 is_running;
	b8 is_suspended;
	platform_state platform;
	i16 width;
	i16 height;
} application_state;

static b8 initialized = FALSE;
static application_state app_state;

b8 application_create(game* game_inst) {
	if (initialized) {
		EN_ERROR("application_create called more than once");
		return FALSE;
	}

	app_state.game_inst = game_inst;

	initialize_logging();

	EN_FATAL("Message: %f", 3.13131f);
	EN_ERROR("Message: %f", 3.13131f);
	EN_WARN("Message: %f", 3.13131f);
	EN_DEBUG("Message: %f", 3.13131f);
	EN_INFO("Message: %f", 3.13131f);
	EN_TRACE("Message: %f", 3.13131f);

	app_state.is_running = TRUE;
	app_state.is_suspended = FALSE;

	if (!event_initialize()) {
		EN_ERROR("Event system was not initialized!"); 
		return FALSE;
	}


	if (!platform_startup(&app_state.platform,
		game_inst->app_config.name,
		game_inst->app_config.start_pos_x,
		game_inst->app_config.start_pos_y,
		game_inst->app_config.start_width,
		game_inst->app_config.start_height)) {
		return FALSE;
	}

	if (!app_state.game_inst->initialize(app_state.game_inst)) {
		EN_FATAL("Game failed to initialize!");
		return FALSE;
	}

	app_state.game_inst->on_resize(app_state.game_inst, app_state.width, app_state.height);

	initialized = TRUE;
	return TRUE;
}

b8 application_run() {
	EN_WARN(get_memory_usage_str());

	while (app_state.is_running) {
		if (!platform_pump_messages(&app_state.platform)) {
			app_state.is_running = FALSE;
		}

		if (!app_state.is_suspended) {

			if (!app_state.game_inst->update(app_state.game_inst, 0)) {
				EN_FATAL("Game update failed, shutting down");
				app_state.is_running = FALSE;
				break;
			}

			if (!app_state.game_inst->render(app_state.game_inst, 0)) {
				EN_FATAL("Game render failed, shutting down.");
				app_state.is_running = FALSE;
				break;
			}


		}
	}

	app_state.is_running = FALSE;
	event_shutdown();
	platform_shutdown(&app_state.platform);
	return TRUE;
}