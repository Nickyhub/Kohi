#pragma once
#include "renderer/renderer_types.inl"

typedef struct geometry_system_config {
	// Max number of geometries that can be loaded at once.
	// NOTE: should be significantly greater than the number of static mehseh because
	// the there can and will be more than one of these per mesh.
	// Take other systems into accout as well.
	u32 max_geometry_count;
} geometry_system_config;

typedef struct geometry_config {
	u32 vertex_size;
	u32 vertex_count;
	void* vertices;
	u32 index_size;
	u32 index_count;
	void* indices;
	char name[GEOMETRY_NAME_MAX_LENGTH];
	char material_name[MATERIAL_NAME_MAX_LENGTH];
} geometry_config;

#define DEFAULT_GEOMETRY_NAME "default"

b8 geometry_system_initialize(u64* memory_requirement, void* state, geometry_system_config config);
void geometry_system_shutdown(void* state);

geometry* geometry_system_acquire_by_id(u32 id);

geometry* geometry_system_acquire_from_config(geometry_config config, b8 auto_release);

void geometry_system_release(geometry* geometry);

geometry* geometry_system_get_default();
geometry* geomtery_system_get_default_2d();

// Generates configuration for plane geometries given the provided parameters.
// NOTE: vertex and index arraya are dynamically allocated and should be freed upon object disposal.
// Thus, this should not be considered production code.

// width: the overall width of the plane. Must be non-zero.
// height: the overall height of the plan. Must be non-zero.
// x_segment_count: the number of segments along the x-axis in the plane. Must be non-zero.
// y_segment_count: the number of segments along the y-axis in the plane. Must be non-zero.
// tile_x: the muber of times the texture should tile across the plan on the x-axis. Must be non-zero.
// tile_y: the muber of times the texture should tile across the plan on the y-axis. Must be non-zero.
// name: The name of the generated geometry.
// material_name: The name of the material to be use.
// return: A gemoetry configuration which can then be fed into geometry_system_acquire_from_config().

geometry_config geometry_system_generate_plane_config(f32 width, f32 height, u32 x_segment_count, u32 y_segment_count, f32 tile_x, f32 tile_y, const char* name, const char* material_name);
