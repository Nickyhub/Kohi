#include "core/application.h"
#include "platform/platform.h"
#include "logger.h"
#include "gametypes.h"
#include "core/kmemory.h"
#include "core/event.h"
#include "core/input.h"

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

b8 application_on_event(u16 code, void* sender, void* listener_inst, event_context context);
b8 application_on_key(u16 code, void* sender, void* listener_inst, event_context context);
b8 application_on_key2(u16 code, void* sender, void* listener_inst, event_context context);

b8 application_create(game* game_inst) {
	if (initialized) {
		EN_ERROR("application_create called more than once");
		return FALSE;
	}

	app_state.game_inst = game_inst;

	initialize_logging();
	input_initialize();

	app_state.is_running = TRUE;
	app_state.is_suspended = FALSE;

	if (!event_initialize()) {
		EN_ERROR("Event system failed initialization. Application cannot continue.");
		return FALSE;
	}

	event_register(EVENT_CODE_APPLICATION_QUIT, 0, application_on_event);
	event_register(EVENT_CODE_KEY_PRESSED, 0, application_on_key);
	event_register(EVENT_CODE_KEY_RELEASED, 0, application_on_key);

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
			//NOTE Inpute update/state copying should always be handled
			//after any input should be recorded.; I.E. before this line.
			//As a safety, input is the last thing to be updated beroe
			//this frame ends.
			input_update(0);
		}
	}

	app_state.is_running = FALSE;

	event_unregister(EVENT_CODE_APPLICATION_QUIT, 0, application_on_event);
	event_unregister(EVENT_CODE_KEY_PRESSED, 0, application_on_key);
	event_unregister(EVENT_CODE_KEY_RELEASED, 0, application_on_key);

	event_shutdown();
	input_shutdown();
	platform_shutdown(&app_state.platform);
	return TRUE;
}

b8 application_on_event(u16 code, void* sender, void* listener_inst, event_context context) {
	switch (code) {
	case EVENT_CODE_APPLICATION_QUIT:
		EN_INFO("EVENT_CODE_APPLICATION_QUIT received, shutting down.\n");
		app_state.is_running = FALSE;
		return TRUE;
	}
}

b8 application_on_key(u16 code, void* sender, void* listener_inst, event_context context) {
	if (code == EVENT_CODE_KEY_PRESSED) {
		u16 key_code = context.data.u16[0];
		if (key_code == KEY_ESCAPE) {
			event_context data = { 0 };
			event_fire(EVENT_CODE_APPLICATION_QUIT, 0, data);
			return TRUE;
		}
		else if (key_code == KEY_A) {
			EN_DEBUG("Explicit - A key pressed!");
		}
		else {
			EN_DEBUG("'%c' pressed in window.", key_code);
		}
	}
	else if (code == EVENT_CODE_KEY_RELEASED) {
		u16 key_code = context.data.u16[0];
		if (key_code == KEY_B) {
			EN_DEBUG("Explicit - B key released!");
		}
		else {
			EN_DEBUG("'%c' key released in window.", key_code);
		}
	}
	return FALSE;
}

b8 application_on_key2(u16 code, void* sender, void* listener_inst, event_context context) {
	EN_FATAL("komische Registrierung funktioniert, guck halt mal im Debugger du Spast!!!!!!!");
	return FALSE;
}
