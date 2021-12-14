#pragma once
#include "defines.h"

/*Represents a simple hastable.Members of this structure
should not be modified outside the functions associated with it. 
For non pointer types, table retains a copy of the value.
For pointer types, make sure to use the _ptr setter and getter. Table
does not take ownership of pointers associated memory allocations,
and should be managed externally. */

typedef struct hashtable {
	u64 element_size;
	u32 element_count;
	b8 is_pointer_type;
	void* memory;
}hashtable;

KAPI void hashtable_create(u64 element_size, u32 element_count, void* memory, b8 is_pointer_type, hashtable* out_hashtable);

KAPI void hashtable_destroy(hashtable* table);

KAPI b8 hashtable_set(hashtable* table, const char* name, void* value);

KAPI b8 hashtable_set_ptr(hashtable* table, const char* name, void** value);

KAPI b8 hashtable_get(hashtable* table, const char* name, void* out_value);

KAPI b8 hashtable_get_ptr(hashtable* table, const char* name, void** out_value); 

KAPI b8 hashtable_fill(hashtable* table, void* value);