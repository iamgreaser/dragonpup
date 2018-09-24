#include "common.h"
#include "world.h"

////////////////////////////////////////////////////////////////////////////

World *new_world(void)
{
	World *world = malloc(sizeof(World));

	memset(world, 0, sizeof(*world));

	world->version_magic = ZZT_WORLD_VERSION;
	world->player_health = 100;
	strncpy(world->world_name.dat,
		"UNTITLED",
		sizeof(world->world_name.dat)-1);
	world->world_name.len = strlen(world->world_name.dat);
#if SUPER_ZZT
	// NOTE: FOREST and PROVING set this to -29801
	world->player_stones = -1; // ?
#endif /* SUPER_ZZT */

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

	tap_ok(world->version_magic == ZZT_WORLD_VERSION,
		"World version is correct");
	tap_ok(world->player_ammo == 0,
		"New world ammo");
	tap_ok(world->player_gems == 0,
		"New world gems");
	assert(sizeof(world->player_keys) ==
		sizeof(world->player_keys[0])*ZZT_MAX_KEYS);
	for(int i = 0; i < ZZT_MAX_KEYS; i++)
	{
		tap_ok(world->player_keys[i] == 0,
			"New world keys");
	}
	tap_ok(world->player_health == 100,
		"New world health");

	tap_ok(world->player_score == 0,
		"New world score");
	tap_ok(world->energy_cycles == 0,
		"New world energy cycles");
#if !SUPER_ZZT
	tap_ok(world->player_torches == 0,
		"New world torches");
	tap_ok(world->torch_cycles == 0,
		"New world torch cycles");
#endif /* !SUPER_ZZT */
	tap_ok(!strcmp(world->world_name.dat, "UNTITLED"),
		"New world name contents");
	tap_ok(world->world_name.len == strlen(world->world_name.dat),
		"New world name length");
	assert(sizeof(world->flags) ==
		sizeof(world->flags[0])*ZZT_MAX_FLAGS);
	for(int i = 0; i < ZZT_MAX_FLAGS; i++)
	{
		tap_ok(world->flags[i].dat[0] == '\x00',
			"Flag empty");
		tap_ok(world->flags[i].len == strlen(world->flags[i].dat),
			"Flag length matches string length");
	}
	tap_ok(world->time_passed == 0,
		"New world time passed");
	tap_ok(world->time_passed_subticks == 0,
		"New world time passed subticks");
	tap_ok(world->is_saved_game == 0,
		"New world is not a save game");
#if SUPER_ZZT
	tap_ok(world->player_stones == -1,
		"New world stones");
#endif /* SUPER_ZZT */

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
