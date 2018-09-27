#include "common.h"
#include "board.h"
#include "block.h"
#include "stat.h"
#include "io.h"

////////////////////////////////////////////////////////////////////////////

Board *new_board(void)
{
	Board *board = malloc(sizeof(Board));

	memset(board, 0, sizeof(*board));

	board->max_player_shots = 255;
	strncpy(board->name.dat, "Untitled", sizeof(board->name.dat)-1);
	board->name.len = strlen(board->name.dat);

	board->block = new_block(
		ZZT_BOARD_WIDTH,
		ZZT_BOARD_HEIGHT);

	int player_x = ZZT_BOARD_WIDTH / 2 - 1;
	int player_y = ZZT_BOARD_HEIGHT / 2 - 1;

	set_block_tile_raw_type(
		board->block, player_x, player_y, T_PLAYER);
	add_stat_to_block(
		board->block, player_x, player_y);

	return board;
}

void free_board(Board **pboard)
{
	if(*pboard != NULL)
	{
		if((*pboard)->block != NULL)
		{
			free((*pboard)->block);
			(*pboard)->block = NULL;
		}

		free(*pboard);
	}

	*pboard = NULL;
}

static void TEST_new_board(void)
{
	Board *board;

	// ZZT-sized board board
	board = new_board();
	tap_ok(board != NULL, "Create board");
	tap_ok(board->block != NULL, "Board has block");
	assert(board->block != NULL);
	tap_ok(board->block->width == ZZT_BOARD_WIDTH,
		"Board width");
	tap_ok(board->block->height == ZZT_BOARD_HEIGHT,
		"Board height");

	tap_ok(!strcmp(board->name.dat, "Untitled"),
		"New board name contents");
	tap_ok(board->name.len == strlen(board->name.dat),
		"New board name length");
	tap_ok(board->max_player_shots == 255,
		"New board player shots");

#if !SUPER_ZZT
	tap_ok(board->is_dark == 0,
		"New board is not dark");
#endif /* !SUPER_ZZT */
	for(int i = 0; i < ZZT_BOARD_EXITS; i++)
	{
		tap_ok(board->exits[i] == 0,
			"New board exits are empty");
	}
	tap_ok(board->restart_on_zap == 0,
		"New board does not restart on zap");
#if !SUPER_ZZT
	tap_ok(board->message.len == 0,
		"New board has no message");
	tap_ok(board->message.len == strlen(board->message.dat),
		"New board message length");
#endif /* !SUPER_ZZT */
#if SUPER_ZZT
	// TODO: camera_x, camera_y
#endif /* SUPER_ZZT */
	tap_ok(board->time_limit == 0,
		"New board has no time limit");

	tap_ok(board->block->stat_count == 1, "Board has 1 stat");
	tap_ok(board->block->stats != NULL, "Stats array exists");
	assert(board->block->stats != NULL);

	// Default player positions from editor:
	// ZZT: 29, 11
	// Super ZZT: 47, 39
#if SUPER_ZZT
	tap_ok(board->block->stats[0]->x == 47, "Default player X correct");
	tap_ok(board->block->stats[0]->y == 39, "Default player Y correct");
#else /* !SUPER_ZZT */
	tap_ok(board->block->stats[0]->x == 29, "Default player X correct");
	tap_ok(board->block->stats[0]->y == 11, "Default player Y correct");
#endif /* SUPER_ZZT */

	free_board(&board);
	tap_ok(true, "Board can be freed");
	tap_ok(board == NULL, "Board set to NULL when freed");
	free_board(&board);
	tap_ok(true, "Freeing NULL boards causes no pain");
	tap_ok(board == NULL, "NULL board stays NULL");
}

////////////////////////////////////////////////////////////////////////////

Board *read_board(IoStream *stream)
{
	Board *board = malloc(sizeof(Board));

	memset(board, 0, sizeof(*board));

	board->block = new_block(
		ZZT_BOARD_WIDTH,
		ZZT_BOARD_HEIGHT);

	ssize_t board_length = io_read_s16le(stream);

	assert(board_length >= 0);
	// ZZT engine limit
	assert(board_length <= 20000);

	// Form substream
	uint8_t *subbuf = malloc(board_length);
	ssize_t subbuf_len = io_read(stream, subbuf, board_length);
	assert(subbuf_len >= 0);
	IoStream *substream = io_open_shared_buffer_for_reading(
		subbuf, subbuf_len);

	//
	// Now start reading from it!
	//

	board->name.len = io_read_u8(substream);
	io_read(substream, board->name.dat, sizeof(board->name.dat)-1);
	if(board->name.len >= sizeof(board->name.dat)-1)
	{
		board->name.len = sizeof(board->name.dat)-1;
	}
	board->name.dat[board->name.len] = 0;

	int rle_count = 0;
	int rle_type = 0;
	int rle_color = 0;
	for(int y = 0; y < board->block->height; y++)
	for(int x = 0; x < board->block->width; x++)
	{
		if(rle_count == 0)
		{
			rle_count = io_read_u8(substream);
			rle_type = io_read_u8(substream);
			rle_color = io_read_u8(substream);
		}
		set_block_tile_raw_type(board->block, x, y, (tile_type)rle_type);
		set_block_tile_raw_color(board->block, x, y, rle_color);
		rle_count--;
		rle_count &= 0xFF;
	}

	board->max_player_shots = io_read_u8(substream);
#if !SUPER_ZZT
	board->is_dark = io_read_u8(substream);
#endif
	for(int i = 0; i < 4; i++)
	{
		board->exits[i] = io_read_u8(substream);
	}
	board->restart_on_zap = io_read_u8(substream);
	// TODO: More stuff

	//
	// Clean up substream
	//
	io_close(&substream);
	free(subbuf); subbuf = NULL;

	return board;
}

static void TEST_read_board(void)
{
	uint8_t fixture[] = {
		0x00, 0x00, // Board size (to be filled in)

		// Board name
		5,
		'B', 'O', 'A', 'R', 'D', 'S', 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
#if SUPER_ZZT
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
#endif /* SUPER_ZZT */

		// Tiles:
		// Tiles ([0,1], 0): Yellow Normal
		2,   T_NORMAL, 0x0E,
		// Tile (2, 0): Player
		1,   T_PLAYER, 0x1F,
		// Most of the remainder is empty
#if SUPER_ZZT
		255, T_EMPTY, 0x70,
		255, T_EMPTY, 0x70,
		255, T_EMPTY, 0x70,
		255, T_EMPTY, 0x70,
		255, T_EMPTY, 0x70,
		255, T_EMPTY, 0x70,
		255, T_EMPTY, 0x70,
		255, T_EMPTY, 0x70,
		255, T_EMPTY, 0x70,
		255, T_EMPTY, 0x70,
		255, T_EMPTY, 0x70,
		255, T_EMPTY, 0x70,
		255, T_EMPTY, 0x70,
		255, T_EMPTY, 0x70,
		255, T_EMPTY, 0x70,
		255, T_EMPTY, 0x70,
		255, T_EMPTY, 0x70,
		255, T_EMPTY, 0x70,
		255, T_EMPTY, 0x70,
		255, T_EMPTY, 0x70,
		255, T_EMPTY, 0x70,
		255, T_EMPTY, 0x70,
		255, T_EMPTY, 0x70,
		255, T_EMPTY, 0x70,
		255, T_EMPTY, 0x70,
		255, T_EMPTY, 0x70,
		255, T_EMPTY, 0x70,
		255, T_EMPTY, 0x70,
		255, T_EMPTY, 0x70,
		0,   T_EMPTY, 0x70,
		24,  T_EMPTY, 0x70,
#else /* !SUPER_ZZT */
		255, T_EMPTY, 0x70,
		255, T_EMPTY, 0x70,
		255, T_EMPTY, 0x70,
		255, T_EMPTY, 0x70,
		0,   T_EMPTY, 0x70,
		219, T_EMPTY, 0x70,
#endif /* SUPER_ZZT */
		// Bottom right corner, two tiles
		1,   T_AMMO,  0x03,
		1,   T_EMPTY, 0x70,

		255, // Max player shots
#if !SUPER_ZZT
		0, // Is dark?
#endif /* !SUPER_ZZT */
		2, 3, 4, 5, // Exits (N, S, W, E)
		0, // Restart on zap?

#if !SUPER_ZZT
		// MESSAGE - TODO
		7,
		'M', 'E', 'S', 'S', 'A', 'G', 'E', 'S', 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
#endif /* !SUPER_ZZT */

		3, 1, // Player enter x, y
#if SUPER_ZZT
		0, 0, // Camera x, y
#endif /* SUPER_ZZT */

		0, 0, // Time limit
		// Padding
#if SUPER_ZZT
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,
#else /* !SUPER_ZZT */
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
#endif /* SUPER_ZZT */

		0,0, // Stat element count

		// Stat 0: Player
		3, 1, // Location x, y
		0, 0, // Step x, y
		1, 0, // Cycle
		0, 0, 0, // P1, P2, P3
		255, 255, // Follower
		255, 255, // Leader
		T_EMPTY, // Under ID
		0x70, // Under colour

		0,0,0,0, // (pointer ignored at load time)
		0,0, // Current instruction
		3,0, // Code length

#if !SUPER_ZZT
		// More padding
		0,0,0,0,0,0,0,0,
#endif /* !SUPER_ZZT */

		// Code
		'@', 'h', 'i',
	};

	// Fill in length
	fixture[0] = 0xFF & (sizeof(fixture));
	fixture[1] = 0xFF & (sizeof(fixture) >> 8);

	IoStream *stream = io_open_shared_buffer_for_reading(
		fixture, sizeof(fixture));
	assert(stream != NULL);
	Board *board = read_board(stream);
	tap_ok(board != NULL, "Read a board");
	io_close(&stream);
	tap_ok(stream == NULL, "Freed I/O stream");

	tap_ok(!strcmp(board->name.dat, "BOARD"),
		"Read board name contents");
	tap_ok(board->name.len == strlen(board->name.dat),
		"Read board name length");

	tap_ok(get_block_tile_raw_type(board->block, 0, 0) == T_NORMAL,
		"Read board tile (0, 0) is a normal");
	tap_ok(get_block_tile_raw_color(board->block, 0, 0) == 0x0E,
		"Read board tile (0, 0) is yellow");
	tap_ok(get_block_tile_raw_type(board->block, 1, 0) == T_NORMAL,
		"Read board tile (1, 0) is a normal");
	tap_ok(get_block_tile_raw_color(board->block, 1, 0) == 0x0E,
		"Read board tile (1, 0) is yellow");
	tap_ok(get_block_tile_raw_type(board->block, 2, 0) == T_PLAYER,
		"Read board tile (2, 0) is a player");
	tap_ok(get_block_tile_raw_color(board->block, 2, 0) == 0x1F,
		"Read board tile (2, 0) is white on blue");
	tap_ok(get_block_tile_raw_type(board->block, 3, 0) == T_EMPTY,
		"Read board tile (3, 0) is empty");
	tap_ok(get_block_tile_raw_type(board->block, 0, 1) == T_EMPTY,
		"Read board tile (0, 1) is empty");
	tap_ok(get_block_tile_raw_type(board->block,
		ZZT_BOARD_WIDTH-2, ZZT_BOARD_HEIGHT-1) == T_AMMO,
		"Read board tile (W-2, H-1) is ammo");
	tap_ok(get_block_tile_raw_color(board->block,
		ZZT_BOARD_WIDTH-2, ZZT_BOARD_HEIGHT-1) == 0x03,
		"Read board tile (W-2, H-1) is dark cyan");
	tap_ok(get_block_tile_raw_type(board->block,
		ZZT_BOARD_WIDTH-1, ZZT_BOARD_HEIGHT-1) == T_EMPTY,
		"Read board tile (W-1, H-1) is empty");

	tap_ok(board->max_player_shots == 255,
		"Read board max player shots");
#if !SUPER_ZZT
	tap_ok(board->is_dark == 0,
		"Read board darkness");
#endif /* !SUPER_ZZT */
	for(int i = 0; i < ZZT_BOARD_EXITS; i++)
	{
		tap_ok(board->exits[i] == 2+i, "Read board exit");
	}
	tap_ok(board->restart_on_zap == 0, "Read board restart on zap");

	// TODO: get this working
	//tap_ok(board->block->stat_count == 1,
	//	"Read board has 1 stat");

	free_board(&board);
	tap_ok(board == NULL, "Freed read board");
}

////////////////////////////////////////////////////////////////////////////

void board_tests(void)
{
	tap_ok(true, "Board test hookup check");
	TEST_new_board();
	TEST_read_board();
}

