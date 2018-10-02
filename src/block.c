#include "common.h"
#include "block.h"
#include "stat.h"
#include "tile.h"

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

int get_stat_index_at_block_since(Block *block, int x, int y, int prev)
{
	for(int i = prev+1; i < block->stat_count; i++)
	{
		if(block->stats[i]->x == x && block->stats[i]->y == y)
		{
			return i;
		}
	}

	return -1;
}

static void TEST_get_stat_index_at_block_since(void)
{
	Block *block = new_block(3, 3);

	// Add one stat
	assert(block != NULL);
	set_block_tile_raw_type(block, 2, 1, T_PLAYER);
	int sidx0 = add_stat_to_block(block, 2, 1);
	Stat *stat0 = block->stats[sidx0];
	set_block_tile_raw_type(block, 0, 1, T_LION);
	int sidx1 = add_stat_to_block(block, 0, 1);
	Stat *stat1 = block->stats[sidx1];
	int sidx2 = add_stat_to_block(block, 2, 1);
	Stat *stat2 = block->stats[sidx2];

	tap_ok(get_stat_index_at_block_since(block, 1, 1, -1) == -1,
		"Stat block index for no stat is -1");
	tap_ok(get_stat_index_at_block_since(block, 2, 1, -1) == sidx0,
		"Stat block index for player");
	tap_ok(get_stat_index_at_block_since(block, 0, 1, -1) == sidx1,
		"Stat block index for lion");
	tap_ok(get_stat_index_at_block_since(block, 2, 1, sidx0) == sidx2,
		"Stat block index for player after first player");
	tap_ok(get_stat_index_at_block_since(block, 0, 1, sidx1) == -1,
		"Stat block index for lion after first player");
	tap_ok(get_stat_index_at_block_since(block, 2, 1, sidx2) == -1,
		"Stat block index for player after second player");

	free_block(&block);
}

////////////////////////////////////////////////////////////////////////////

// By the way, this is located at the very end of the EXE on both counts.
static const uint8_t line_chars[16] = {
	0xF9, 0xD0, 0xD2, 0xBA,
	0xB5, 0xBC, 0xBB, 0xB9,
	0xC6, 0xC8, 0xC9, 0xCC,
	0xCD, 0xCA, 0xCB, 0xCE,
};

#if SUPER_ZZT
// This, by the way, is the string before the line table.
static const uint8_t web_chars[16] = {
	0xFA, 0xB3, 0xB3, 0xB3,
	0xC4, 0xD9, 0xBF, 0xB4,
	0xC4, 0xC0, 0xDA, 0xC3,
	0xC4, 0xC1, 0xC2, 0xC5,
};
#endif /* SUPER_ZZT */

uint8_t get_block_tile_visible_char(Block *block, int x, int y)
{
	if(!coords_in_range_of_block(block, x, y))
	{
		return 0x20;
	}

	assert(x >= 0);
	assert(y >= 0);
	assert(x < block->width);
	assert(y < block->height);

	Tile *tile = &block->tile_data[x*block->height + y];
	Stat *stat = NULL;

	if(tile->type == T_LINE)
	{
		tile_type tyn = get_block_tile_raw_type(block, x, y-1);
		tile_type typ = get_block_tile_raw_type(block, x, y+1);
		tile_type txn = get_block_tile_raw_type(block, x-1, y);
		tile_type txp = get_block_tile_raw_type(block, x+1, y);

		int cidx = 0;
		if(tyn == T_LINE || tyn == T_BOARD_EDGE) { cidx |= 0x1; }
		if(typ == T_LINE || typ == T_BOARD_EDGE) { cidx |= 0x2; }
		if(txn == T_LINE || txn == T_BOARD_EDGE) { cidx |= 0x4; }
		if(txp == T_LINE || txp == T_BOARD_EDGE) { cidx |= 0x8; }

		assert(cidx >= 0 && cidx < 16);
		return line_chars[cidx];
	}

#if SUPER_ZZT
	if(tile->type == T_WEB)
	{
		tile_type tyn = get_block_tile_raw_type(block, x, y-1);
		tile_type typ = get_block_tile_raw_type(block, x, y+1);
		tile_type txn = get_block_tile_raw_type(block, x-1, y);
		tile_type txp = get_block_tile_raw_type(block, x+1, y);

		int cidx = 0;
		if(tyn == T_WEB || tyn == T_BOARD_EDGE) { cidx |= 0x1; }
		if(typ == T_WEB || typ == T_BOARD_EDGE) { cidx |= 0x2; }
		if(txn == T_WEB || txn == T_BOARD_EDGE) { cidx |= 0x4; }
		if(txp == T_WEB || txp == T_BOARD_EDGE) { cidx |= 0x8; }

		assert(cidx >= 0 && cidx < 16);
		return web_chars[cidx];
	}
#endif /* SUPER_ZZT */

	return get_tile_char(tile, stat, block, x, y);
}

uint8_t get_block_tile_visible_color(Block *block, int x, int y)
{
	if(!coords_in_range_of_block(block, x, y))
	{
		return 0x10;
	}

	assert(x >= 0);
	assert(y >= 0);
	assert(x < block->width);
	assert(y < block->height);

	Tile *tile = &block->tile_data[x*block->height + y];
	Stat *stat = NULL;

	return get_tile_color(tile, stat, block, x, y);
}

static void TEST_get_block_tile_visible_char_color(void)
{
	Block *block = new_block(12, 10);
	assert(block != NULL);

	set_block_tile_raw_type(block, 0, 0, T_PLAYER);
	set_block_tile_raw_color(block, 0, 0, 0x1F);
	set_block_tile_raw_type(block, 1, 0, T_EMPTY);
	set_block_tile_raw_color(block, 1, 0, 0x70);
	set_block_tile_raw_type(block, 2, 0, T_LION);
	set_block_tile_raw_color(block, 2, 0, 0x0C);
	set_block_tile_raw_type(block, 3, 0, T_LION);
	set_block_tile_raw_color(block, 3, 0, 0x06);

	set_block_tile_raw_type(block, 0, 1, T_TEXT_RED);
	set_block_tile_raw_color(block, 0, 1, 0x21);
	set_block_tile_raw_type(block, 1, 1, T_TEXT_BLUE);
	set_block_tile_raw_color(block, 1, 1, 0x21);
	set_block_tile_raw_type(block, 2, 1, T_TEXT_BLUE);
	set_block_tile_raw_color(block, 2, 1, 0x3F);
	set_block_tile_raw_type(block, 3, 1, T_TEXT_BLACK);
	set_block_tile_raw_color(block, 3, 1, 0x21);

	// Get some line tests in.
	for(int y = 4; y <= 6; y++)
	{
		for(int x = 4; x <= 6; x++)
		{
			set_block_tile_raw_type(block, x, y, T_LINE);
			set_block_tile_raw_color(block, x, y, y);
		}
	}

#if SUPER_ZZT
	// Get some web tests in too.
	for(int y = 1; y <= 3; y++)
	{
		for(int x = 7; x <= 9; x++)
		{
			set_block_tile_raw_type(block, x, y, T_WEB);
			set_block_tile_raw_color(block, x, y, y);
		}
	}
#endif /* !SUPER_ZZT */

	tap_ok(get_block_tile_visible_char(block, 0, 0) == 0x02,
		"Tile char: Player");
	tap_ok(get_block_tile_visible_color(block, 0, 0) == 0x1F,
		"Tile color: Player");
	tap_ok(get_block_tile_visible_char(block, 1, 0) == 0x20,
		"Tile char: Empty");
	tap_ok(get_block_tile_visible_color(block, 1, 0) == 0x07,
		"Tile color: Empty");
	tap_ok(get_block_tile_visible_char(block, 2, 0) == 0xEA,
		"Tile char: Lion Red");
	tap_ok(get_block_tile_visible_color(block, 2, 0) == 0x0C,
		"Tile color: Lion Red");
	tap_ok(get_block_tile_visible_char(block, 3, 0) == 0xEA,
		"Tile char: Lion Brown");
	tap_ok(get_block_tile_visible_color(block, 3, 0) == 0x06,
		"Tile color: Lion Brown");

	// Yep, more text.
	tap_ok(get_block_tile_visible_char(block, 0, 1) == 0x21,
		"Tile char: Text Red, 0x21");
	tap_ok(get_block_tile_visible_color(block, 0, 1) == 0x4F,
		"Tile color: Text Red, 0x21");
	tap_ok(get_block_tile_visible_char(block, 1, 1) == 0x21,
		"Tile char: Text Blue, 0x21");
	tap_ok(get_block_tile_visible_color(block, 1, 1) == 0x1F,
		"Tile color: Text Blue, 0x21");
	tap_ok(get_block_tile_visible_char(block, 2, 1) == 0x3F,
		"Tile char: Text Blue, 0x3F");
	tap_ok(get_block_tile_visible_color(block, 2, 1) == 0x1F,
		"Tile color: Text Blue, 0x3F");
	tap_ok(get_block_tile_visible_char(block, 3, 1) == 0x21,
		"Tile char: Text Black, 0x21");
	tap_ok(get_block_tile_visible_color(block, 3, 1) == 0x0F,
		"Tile color: Text Black, 0x21");

	// Lines!
	// Every char must be different.
	for(int y = 4; y <= 6-1; y++)
	{
		for(int x = 4; x <= 6-1; x++)
		{
			int c00 = get_block_tile_visible_char(block, x+0, y+0);
			int c01 = get_block_tile_visible_char(block, x+0, y+1);
			int c10 = get_block_tile_visible_char(block, x+1, y+0);
			int c11 = get_block_tile_visible_char(block, x+1, y+1);
			tap_ok(c00 != c01, "Line char differences 01");
			tap_ok(c00 != c10, "Line char differences 10");
			tap_ok(c00 != c11, "Line char differences 11");
		}
	}

#if SUPER_ZZT
	// Webs!
	// Every char must be different.
	for(int y = 1; y <= 3-1; y++)
	{
		for(int x = 7; x <= 9-1; x++)
		{
			int cline = get_block_tile_visible_char(block, x-3, y+3);
			int c00 = get_block_tile_visible_char(block, x+0, y+0);
			int c01 = get_block_tile_visible_char(block, x+0, y+1);
			int c10 = get_block_tile_visible_char(block, x+1, y+0);
			int c11 = get_block_tile_visible_char(block, x+1, y+1);
			tap_ok(c00 != c01, "Web char differences 01");
			tap_ok(c00 != c10, "Web char differences 10");
			tap_ok(c00 != c11, "Web char differences 11");
			tap_ok(c00 != cline, "Web char != line char");
		}
	}
#endif /* SUPER_ZZT */

	free_block(&block);
}

////////////////////////////////////////////////////////////////////////////

void block_tests(void)
{
	tap_ok(true, "Block test hookup check");
	TEST_new_block();
	TEST_getset_block_tile_raw_type();
	TEST_getset_block_tile_raw_color();
	TEST_add_stat_to_block();
	TEST_get_stat_index_at_block_since();
	TEST_get_block_tile_visible_char_color();
}
