#pragma once

#include "defines.h"
#include "core/asserts.h"
#include <vulkan/vulkan.h>

#define VK_CHECK(expr)                  \
    {                                   \
        EN_ASSERT(expr == VK_SUCCESS);  \
    }

//#define VK_CHECK(expr) {            \
//    EN_ASSERT(expr == VK_SUCCESS);  \
//}

typedef struct vulkan_context {
	VkInstance instance;
	VkAllocationCallbacks* allocator;
} vulkan_context;