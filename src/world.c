#include "common.h"
#include "world.h"

////////////////////////////////////////////////////////////////////////////

World *new_world(void)
{
	World *world = malloc(sizeof(World));

	memset(world, 0, sizeof(world));

	return world;
}

void free_world(World **pworld)
{
	if(*pworld != NULL)
	{
		free(*pworld);
	}

	*pworld = NULL;
}

static void TEST_new_world(void)
{
	World *world;

	// World creation
	world = new_world();
	tap_ok(world != NULL, "Create world");

	free_world(&world);
	tap_ok(true, "World can be freed");
	tap_ok(world == NULL, "World set to NULL when freed");
	free_world(&world);
	tap_ok(true, "Freeing NULL worlds causes no pain");
	tap_ok(world == NULL, "NULL world stays NULL");
}

////////////////////////////////////////////////////////////////////////////

void world_tests(void)
{
	tap_ok(true, "World test hookup check");
	TEST_new_world();
}
