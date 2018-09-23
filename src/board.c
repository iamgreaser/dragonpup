#include "common.h"
#include "board.h"

////////////////////////////////////////////////////////////////////////////

Block *new_block(int width, int height)
{
	Block *block = malloc(sizeof(Block));
	size_t tile_count = width * height;
	size_t tile_bytes = sizeof(Tile) * tile_count;

	memset(block, 0, sizeof(block));
	block->width = width;
	block->height = height;

	block->tile_data = malloc(tile_bytes);
	memset(block->tile_data, 0, tile_bytes);

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

static void TEST_getset_block_tile_raw(void)
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

void board_tests(void)
{
	tap_ok(true, "Board test hookup check");
	TEST_new_block();
	TEST_getset_block_tile_raw();
}

