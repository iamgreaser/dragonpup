#include "common.h"
#include "board.h"

////////////////////////////////////////////////////////////////////////////

Block *new_block(int width, int height)
{
	Block *block = malloc(sizeof(Block));

	memset(block, 0, sizeof(block));

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

	free_block(&block);
	tap_ok(true, "Block can be freed");
	tap_ok(block == NULL, "Block set to NULL when freed");
	free_block(&block);
	tap_ok(true, "Freeing NULL blocks causes no pain");
	tap_ok(block == NULL, "NULL block stays NULL");

	// Super ZZT-sized board block
	block = new_block(96, 80);
	tap_ok(block != NULL, "Create block: (96, 80)");
	free_block(&block);

}

////////////////////////////////////////////////////////////////////////////

void board_tests(void)
{
	tap_ok(true, "Board test hookup check");
	TEST_new_block();
}

