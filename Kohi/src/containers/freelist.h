#pragma once

#include "defines.h"

typedef struct freelist {
	void* memory;
} freelist;

KAPI void freelist_create(u64 total_size, u64* memory_requirement, void* memory, freelist* out_list);

KAPI void freelist_destroy(freelist* list);

KAPI void freelist_destroy(freelist* list);

KAPI b8 freelist_allocate_block(freelist* list, u64 size, u64* out_offset);

KAPI b8 freelist_free_block(freelist* list, u64 size, u64 offset);

KAPI void freelist_clear(freelist* list);

KAPI u64 freelist_free_space(freelist* list);