#include "common.h"
#include "board.h"
#include "io.h"
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

	world->board_count = 1;
	world->boards = malloc(sizeof(Board *) * world->board_count);
	for(int i = 0; i < world->board_count; i++)
	{
		world->boards[i] = new_board();
	}

	return world;
}

void free_world(World **pworld)
{
	if(*pworld != NULL)
	{
		assert((*pworld)->boards != NULL);

		for(int i = 0; i < (*pworld)->board_count; i++)
		{
			free_board(&((*pworld)->boards[i]));
		}

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

	tap_ok(world->board_count == 1,
		"World has 1 board");
	tap_ok(world->boards != NULL,
		"World has a board array");
	assert(world->boards != NULL);
	tap_ok(world->boards[0] != NULL,
		"World board 0 exists");

	free_world(&world);
	tap_ok(true, "World can be freed");
	tap_ok(world == NULL, "World set to NULL when freed");
	free_world(&world);
	tap_ok(true, "Freeing NULL worlds causes no pain");
	tap_ok(world == NULL, "NULL world stays NULL");
}

////////////////////////////////////////////////////////////////////////////

World *read_world(IoStream *stream)
{
	int version_magic = io_read_s16le(stream);

	if(version_magic != ZZT_WORLD_VERSION)
	{
		return NULL;
	}

#if SUPER_ZZT
	uint8_t header_buf[0x400-2];
#else /* !SUPER_ZZT */
	uint8_t header_buf[0x200-2];
#endif /* SUPER_ZZT */
	io_read(stream, header_buf, sizeof(header_buf));
	IoStream *header_stream = io_open_shared_buffer_for_reading(
		header_buf, sizeof(header_buf));
	World *world = malloc(sizeof(World));
	memset(world, 0, sizeof(*world));
	world->version_magic = version_magic;
	world->board_count = io_read_s16le(header_stream) + 1;
#if 0
	world->player_ammo = io_read_s16le(header_stream);
	world->player_gems = io_read_s16le(header_stream);
	for(int i = 0; i < ZZT_MAX_KEYS; i++)
	{
		world->player_keys[i] = io_read_u8(header_stream);
	}
	int16_t player_health;
	int16_t starting_board;

#if SUPER_ZZT
	int16_t player_score;
	int16_t energy_cycles;
#else /* !SUPER_ZZT */
	int16_t player_torches;
	int16_t torch_cycles;
	int16_t energy_cycles;
	int16_t player_score;
#endif /* SUPER_ZZT */
	pstring20 world_name;
	pstring20 flags[ZZT_MAX_FLAGS];
	int16_t time_passed;
	int16_t time_passed_subticks;
	uint8_t is_saved_game;
#if SUPER_ZZT
	int16_t player_stones;
#endif /* SUPER_ZZT */
#endif
	// TODO: other stuff

	io_close(&header_stream);

	world->boards = malloc(sizeof(Board *) * world->board_count);
	for(int i = 0; i < world->board_count; i++)
	{
		world->boards[i] = read_board(stream);
	}

	return world;
}

static void TEST_read_world(void)
{
	// Load board fixture
	uint8_t *board_fixture_base_buf = NULL;
	size_t board_fixture_base_bytes = 0;
	FIXTURE_read_board(
		&board_fixture_base_buf,
		&board_fixture_base_bytes);
	assert(board_fixture_base_buf != NULL);
	assert(board_fixture_base_bytes != 0);

	// Make world fixture
	uint8_t world_fixture_header[] = {
#if SUPER_ZZT
		0xFE, 0xFF, // Magic number
#else /* !SUPER_ZZT */
		0xFF, 0xFF, // Magic number
#endif /* SUPER_ZZT */
		1, 0, // Number of boards
		20, 0, // Ammo
		1, 0, // Gems
		1, 1, 1, 1, 1, 1, 1, // Keys
		100, 0, // Health
		1, 0, // Starting board
#if SUPER_ZZT
		0, 0, // PADDING
		1, 4, // Player score (0x0401 == 1025)
		0, 0, // PADDING
		0, 0, // Energy cycles
#else /* !SUPER_ZZT */
		3, 0, // Torches
		0, 0, // Torch cycles
		0, 0, // Energy cycles
		0, 0, // PADDING
		1, 4, // Player score (0x0401 == 1025)
#endif /* SUPER_ZZT */

		// World name
		8,
		'U','N','T','I','T','L','E','D','E','D',
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

		// Flags
		//
		// ZZT has 10
		// Super ZZT has 16
		6,
		'S','E','C','R','E','T','L','Y','I','N',
		'L','O','V','E', 0, 0, 0, 0, 0, 0,
		0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
#if SUPER_ZZT
		0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
#endif /* SUPER_ZZT */
		3, 0, // Time passed
		0, 0, // Time passed subseconds
		0, // Save game flag
#if SUPER_ZZT
		5, 0, // Stones
#endif /* SUPER_ZZT */
	};

#if SUPER_ZZT
	const size_t world_header_bytes = 0x400;
#else /* !SUPER_ZZT */
	const size_t world_header_bytes = 0x200;
#endif /* SUPER_ZZT */
	size_t world_fixture_bytes = (
		world_header_bytes
		+ board_fixture_base_bytes * 2);
	uint8_t *world_fixture_buf = malloc(world_fixture_bytes);
	memset(world_fixture_buf, 0, world_fixture_bytes);
	memcpy(world_fixture_buf,
		world_fixture_header,
		sizeof(world_fixture_header));
	for(int i = 0; i < 2; i++)
	{
		memcpy(world_fixture_buf
				+ world_header_bytes
				+ board_fixture_base_bytes * i,
			board_fixture_base_buf,
			board_fixture_base_bytes);
	}

	IoStream *stream = io_open_shared_buffer_for_reading(
		world_fixture_buf, world_fixture_bytes);
	World *world = read_world(stream);
	io_close(&stream);
	tap_ok(world != NULL, "Read a world");
	assert(world != NULL);

	tap_ok(world->board_count == 2, "Read world board count");

	free_world(&world);
	free(world_fixture_buf);
}

////////////////////////////////////////////////////////////////////////////

void world_tests(void)
{
	tap_ok(true, "World test hookup check");
	TEST_new_world();
	TEST_read_world();
}
