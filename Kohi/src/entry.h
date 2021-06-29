#include "core/asserts.h"

#include "core/application.h"

int main() {
	application_config config;
	config.name = "Kohi Engine";
	config.start_pos_x = 100;
	config.start_pos_y = 100;
	config.start_width = 1280;
	config.start_height = 720;

	application_create(&config);
	application_run();

	return 0;
}