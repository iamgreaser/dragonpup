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

void board_tests(void)
{
	tap_ok(true, "Board test hookup check");
	TEST_new_block();
}

