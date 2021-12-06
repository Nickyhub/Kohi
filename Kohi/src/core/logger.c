#include "logger.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "platform/platform.h"
#include "platform/filesystem.h"

typedef struct logger_system_state {
	b8 initialized;
	file_handle log_file_handle;
}logger_system_state;

static logger_system_state* state_ptr;

void append_to_log_file(const char* message) {
	if (state_ptr && state_ptr->log_file_handle.is_valid) {
		u64 length = string_length(message);
		u64 written = 0;
		if (!filesystem_write(&state_ptr->log_file_handle, length, message, &written)) {
			platform_console_write_error("ERROR writing to console.log.", LOG_LEVEL_ERROR);
		}
	}
}

b8 initialize_logging(u64* memory_requirement, void* state) {
	*memory_requirement = sizeof(logger_system_state);

	if (state == 0) {
		return true;
	}

	state_ptr = state;
	state_ptr->initialized = true;
	//TODO create logfile

	if (!filesystem_open("console.log", FILE_MODE_WRITE, false, &state_ptr->log_file_handle)) {
		platform_console_write_error("ERROR: Unable to open console.log for writing.", LOG_LEVEL_ERROR);
		return false;
	}

	return true;
}

void shutdown_logging(void* state) {

	state_ptr = 0;
}

void log_output(enum log_level level, const char* messageformat, ...) {
	const char* level_string[6] = { "[FATAL] ", "[ERROR] ", "[WARNING] ",  "[INFO] ",  "[DEBUG] ", "[TRACE] ", };

	b8 is_error = level < LOG_LEVEL_WARN;

	char buffer[LOG_MESSAGE_LENGTH];
	char out_message[LOG_MESSAGE_LENGTH];

	memset(buffer, 0, sizeof(buffer));
	memset(out_message, 0, sizeof(out_message));

	va_list list;
	va_start(list, messageformat);
	vsnprintf(buffer, LOG_MESSAGE_LENGTH, messageformat, list);
	va_end(list);

	sprintf(out_message, "%s%s\n", level_string[level], buffer);

	if (is_error) {
		platform_console_write_error(out_message, level);
	}
	else {
		platform_console_write(out_message, level);
	}

	append_to_log_file(out_message);

}
