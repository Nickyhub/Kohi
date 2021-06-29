#include "logger.h"

void log_output(enum log_level level, const char* messageformat, ...) {
	const char* level_string[6] = { "[FATAL] ", "[ERROR] ", "[WARNING] ",  "[INFO] ",  "[DEBUG] ", "[TRACE] ", };

	b8 is_error = level < LOG_LEVEL_WARNING;

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

}

b8 initialize_logging() {
	//TODO create logfile
	return TRUE;
}