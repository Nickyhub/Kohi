#include "test_manager.h"
#include "memory/linear_allocator_tests.h"

#include <core/logger.h>

int main() {
	test_manager_init();

	linear_allocator_register_tests();
	//TODO Register tests

	EN_DEBUG("Starting tests...");

	test_manager_run_tests(0);
	return 0;
}