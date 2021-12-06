#pragma  once
#include <defines.h>

void linear_allocator_register_tests();

u8 linear_allocator_multi_allocation_all_space();

u8 linear_allocator_multi_allocation_over_allocate();

u8 linear_allocator_multi_allocation_all_space_then_free();

u8 linear_allocator_single_allocation_all_space();