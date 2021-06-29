#include "application.h"
#include "platform/platform.h"
#include "logger.h"

typedef struct application_state {
	b8 is_running;
	b8 is_suspended;
	platform_state platform;
	i16 width;
	i16 height;
} application_state;

static b8 initialized = FALSE;
static application_state app_state;

KAPI b8 application_create(application_config* config) {
	if (initialized) {
		EN_ERROR("application_create called more than once");
		return FALSE;
	}

	initialize_logging();

	EN_FATAL("Message: %f", 3.13131f);
	EN_ERROR("Message: %f", 3.13131f);
	EN_WARNING("Message: %f", 3.13131f);
	EN_DEBUG("Message: %f", 3.13131f);
	EN_INFO("Message: %f", 3.13131f);
	EN_TRACE("Message: %f", 3.13131f);

	app_state.is_running = TRUE;
	app_state.is_suspended = FALSE;


	if (!platform_startup(&app_state.platform, config->name, config->start_pos_x, config->start_pos_y, config->start_width, config->start_height)) {
		return FALSE;
	}

	initialized = TRUE;
	return TRUE;
}

KAPI b8 application_run() {
	while (app_state.is_running) {
		if (!platform_pump_messages(&app_state.platform)) {
			app_state.is_running = FALSE;
		}
	}

	app_state.is_running = FALSE;

	platform_shutdown(&app_state.platform);
	return TRUE;
}