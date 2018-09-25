#include "common.h"
#include "board.h"
#include "stat.h"

////////////////////////////////////////////////////////////////////////////

Board *new_board(void)
{
	Board *board = malloc(sizeof(Board));

	memset(board, 0, sizeof(*board));

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

	tap_ok(board->block->stat_count == 1, "Board has 1 stat");
	tap_ok(board->block->stats != NULL, "Stats array exists");
	assert(board->block->stats != NULL);

	// Default player positions from editor:
	// ZZT: 29, 11
	// Super ZZT: 47, 39
#if SUPER_ZZT
	tap_ok(board->block->stats[0]->x == 47, "Default player X correct");
	tap_ok(board->block->stats[0]->y == 39, "Default player Y correct");
#else
	tap_ok(board->block->stats[0]->x == 29, "Default player X correct");
	tap_ok(board->block->stats[0]->y == 11, "Default player Y correct");
#endif

	free_board(&board);
	tap_ok(true, "Board can be freed");
	tap_ok(board == NULL, "Board set to NULL when freed");
	free_board(&board);
	tap_ok(true, "Freeing NULL boards causes no pain");
	tap_ok(board == NULL, "NULL board stays NULL");
}


////////////////////////////////////////////////////////////////////////////

Block *new_block(int width, int height)
{
	Block *block = malloc(sizeof(Block));
	size_t tile_count = width * height;
	size_t tile_bytes = sizeof(Tile) * tile_count;

	memset(block, 0, sizeof(*block));
	block->width = width;
	block->height = height;

	block->tile_data = malloc(tile_bytes);
	memset(block->tile_data, 0, tile_bytes);
	for(int y = 0; y < block->height; y++)
	{
		for(int x = 0; x < block->width; x++)
		{
			set_block_tile_raw_type(
				block, x, y, T_EMPTY);
			set_block_tile_raw_color(
				block, x, y, 0x70);
		}
	}

	return block;
}

void free_block(Block **pblock)
{
	if(*pblock != NULL)
	{
		if((*pblock)->tile_data != NULL)
		{
			free((*pblock)->tile_data);
			(*pblock)->tile_data = NULL;
		}

		if((*pblock)->stats != NULL)
		{
			for(int i = 0; i < (*pblock)->stat_count; i++)
			{
				free_stat(&((*pblock)->stats[i]));
			}

			free((*pblock)->stats);
			(*pblock)->stats = NULL;
			(*pblock)->stat_count = 0;
		}

		free(*pblock);
	}

	*pblock = NULL;
}

static void TEST_new_block(void)
{
	Block *block;

	// ZZT-sized board block
	block = new_block(60, 25);
	tap_ok(block != NULL, "Create block: (60, 25)");
	tap_ok(block->width == 60 && block->height == 25, "Block is (60, 25)");
	tap_ok(block->tile_data != NULL, "Block has tile data");

	free_block(&block);
	tap_ok(true, "Block can be freed");
	tap_ok(block == NULL, "Block set to NULL when freed");
	free_block(&block);
	tap_ok(true, "Freeing NULL blocks causes no pain");
	tap_ok(block == NULL, "NULL block stays NULL");

	// Super ZZT-sized board block
	block = new_block(96, 80);
	tap_ok(block != NULL, "Create block: (96, 80)");
	tap_ok(block->width == 96 && block->height == 80, "Block is (96, 80)");
	free_block(&block);

}

////////////////////////////////////////////////////////////////////////////

static bool coords_in_range_of_block(Block *block, int x, int y)
{
	return (x >= 0 && y >= 0
		&& x < block->width
		&& y < block->height);
}

////////////////////////////////////////////////////////////////////////////

tile_type get_block_tile_raw_type(Block *block, int x, int y)
{
	if(!coords_in_range_of_block(block, x, y))
	{
		return T_BOARD_EDGE;
	}

	assert(x >= 0);
	assert(y >= 0);
	assert(x < block->width);
	assert(y < block->height);

	return block->tile_data[x*block->height + y].type;
}

void set_block_tile_raw_type(Block *block, int x, int y, tile_type type)
{
	// TODO!
	if(!coords_in_range_of_block(block, x, y))
	{
		return;
	}

	assert(x >= 0);
	assert(y >= 0);
	assert(x < block->width);
	assert(y < block->height);

	block->tile_data[x*block->height + y].type = type;
}

static void TEST_getset_block_tile_raw_type(void)
{
	Block *block;

	// Get type in range
	block = new_block(60, 25);
	tap_ok(block != NULL, "Create block: (60, 25)");
	tap_ok(get_block_tile_raw_type(block, 0, 0) == T_EMPTY,
		"Block at TLC is empty");
	tap_ok(get_block_tile_raw_type(block, 59, 24) == T_EMPTY,
		"Block at BRC is empty");
	set_block_tile_raw_type(block, 0, 0, T_AMMO);
	tap_ok(get_block_tile_raw_type(block, 0, 0) == T_AMMO,
		"Block at TLC when set to ammo is now ammo");
	tap_ok(get_block_tile_raw_type(block, -1, 2) == T_BOARD_EDGE,
		"Block past left border is edge");
	tap_ok(get_block_tile_raw_type(block, 60, 2) == T_BOARD_EDGE,
		"Block past right border is edge");
	tap_ok(get_block_tile_raw_type(block, 2, -1) == T_BOARD_EDGE,
		"Block past top border is edge");
	tap_ok(get_block_tile_raw_type(block, 2, 25) == T_BOARD_EDGE,
		"Block past bottom border is edge");
	set_block_tile_raw_type(block, -1, 2, T_AMMO);
	set_block_tile_raw_type(block, 60, 2, T_AMMO);
	set_block_tile_raw_type(block, 2, -1, T_AMMO);
	set_block_tile_raw_type(block, 2, 25, T_AMMO);
	tap_ok(get_block_tile_raw_type(block, -1, 2) == T_BOARD_EDGE,
		"Block past left border after mutation is still edge");
	tap_ok(get_block_tile_raw_type(block, 60, 2) == T_BOARD_EDGE,
		"Block past right border after mutation is still edge");
	tap_ok(get_block_tile_raw_type(block, 2, -1) == T_BOARD_EDGE,
		"Block past top border after mutation is still edge");
	tap_ok(get_block_tile_raw_type(block, 2, 25) == T_BOARD_EDGE,
		"Block past bottom border after mutation is still edge");
	free_block(&block);
}

////////////////////////////////////////////////////////////////////////////

uint8_t get_block_tile_raw_color(Block *block, int x, int y)
{
	if(!coords_in_range_of_block(block, x, y))
	{
		return 0x10; // sane default, can be changed
	}

	assert(x >= 0);
	assert(y >= 0);
	assert(x < block->width);
	assert(y < block->height);

	return block->tile_data[x*block->height + y].color;
}

void set_block_tile_raw_color(Block *block, int x, int y, uint8_t color)
{
	// TODO!
	if(!coords_in_range_of_block(block, x, y))
	{
		return;
	}

	assert(x >= 0);
	assert(y >= 0);
	assert(x < block->width);
	assert(y < block->height);

	block->tile_data[x*block->height + y].color = color;
}

////////////////////////////////////////////////////////////////////////////

static void TEST_getset_block_tile_raw_color(void)
{
	Block *block;

	// Get type in range
	block = new_block(60, 25);
	tap_ok(block != NULL, "Create block: (60, 25)");
	tap_ok(get_block_tile_raw_color(block, 0, 0) == 0x70,
		"Block at TLC is colour 0x70");
	tap_ok(get_block_tile_raw_color(block, 59, 24) == 0x70,
		"Block at BRC is colour 0x70");
	set_block_tile_raw_color(block, 0, 0, 0x3B);
	tap_ok(get_block_tile_raw_color(block, 0, 0) == 0x3B,
		"Block at TLC when set to colour 0x3B is now 0x3B");
	tap_ok(get_block_tile_raw_color(block, -1, 2) == 0x10,
		"Block past left border is 0x10");
	tap_ok(get_block_tile_raw_color(block, 60, 2) == 0x10,
		"Block past right border is 0x10");
	tap_ok(get_block_tile_raw_color(block, 2, -1) == 0x10,
		"Block past top border is 0x10");
	tap_ok(get_block_tile_raw_color(block, 2, 25) == 0x10,
		"Block past bottom border is 0x10");
	set_block_tile_raw_color(block, -1, 2, 0x3B);
	set_block_tile_raw_color(block, 60, 2, 0x3B);
	set_block_tile_raw_color(block, 2, -1, 0x3B);
	set_block_tile_raw_color(block, 2, 25, 0x3B);
	tap_ok(get_block_tile_raw_color(block, -1, 2) == 0x10,
		"Block past left border after mutation is still 0x10");
	tap_ok(get_block_tile_raw_color(block, 60, 2) == 0x10,
		"Block past right border after mutation is still 0x10");
	tap_ok(get_block_tile_raw_color(block, 2, -1) == 0x10,
		"Block past top border after mutation is still 0x10");
	tap_ok(get_block_tile_raw_color(block, 2, 25) == 0x10,
		"Block past bottom border after mutation is still 0x10");
	free_block(&block);
}

////////////////////////////////////////////////////////////////////////////

int add_stat_to_block(Block *block, int x, int y)
{
	int sidx = block->stat_count;

	block->stat_count += 1;
	block->stats = realloc(
		block->stats,
		sizeof(block->stats[0]) * block->stat_count);
	block->stats[sidx] = new_stat(
		x, y,
		get_block_tile_raw_type(block, x, y));

	return sidx;
}

void TEST_add_stat_to_block(void)
{
	int sidx0, sidx1;
	Block *block;
	Stat *stat0, *stat1;

	// Add one stat
	block = new_block(3, 3);
	assert(block != NULL);
	set_block_tile_raw_type(block, 2, 1, T_PLAYER);
	tap_ok(block->stat_count == 0,
		"New blocks have no stats");
	tap_ok(block->stats == NULL,
		"If there are no stats, there is no stat array");
	sidx0 = add_stat_to_block(block, 2, 1);
	tap_ok(sidx0 == 0,
		"First stat is at index 0");
	tap_ok(block->stat_count == 1,
		"After adding a stat it must be there");
	tap_ok(block->stats != NULL,
		"If there are stats, there is a stat array");
	assert(block->stats != NULL);
	stat0 = block->stats[sidx0];
	tap_ok(stat0 != NULL,
		"Stat 0 must be in the array");
	assert(stat0 != NULL);
	tap_ok(stat0->x == 2 && stat0->y == 1,
		"Stat 0 must be at its given location of (2, 1)");

	// Add another stat
	set_block_tile_raw_type(block, 0, 1, T_LION);
	sidx1 = add_stat_to_block(block, 0, 1);
	tap_ok(sidx1 == 1,
		"Second stat is at index 1");
	tap_ok(block->stat_count == 2,
		"After adding the second stat it must be there");
	tap_ok(block->stats != NULL,
		"We must still have a stat array");
	assert(block->stats != NULL);
	stat1 = block->stats[sidx1];
	tap_ok(stat1 != NULL,
		"Stat 1 must be in the array");
	assert(stat1 != NULL);
	tap_ok(stat0 == block->stats[sidx0],
		"Stat 0 must still be Stat 0");
	tap_ok(stat1->x == 0 && stat1->y == 1,
		"Stat 1 must be at its given location of (0, 1)");

	free_block(&block);
}

////////////////////////////////////////////////////////////////////////////

void board_tests(void)
{
	tap_ok(true, "Board test hookup check");
	TEST_new_block();
	TEST_getset_block_tile_raw_type();
	TEST_getset_block_tile_raw_color();
	TEST_add_stat_to_block();
	TEST_new_board();
}

