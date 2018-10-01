#include <assert.h>
#include <stdio.h>

#include "common.h"
#include "io.h"
#include "world.h"

// for spikes
#include "block.h"
#include "vt.h"

int main(int argc, char *argv[])
{
	bool quit_after_tests = false;
	bool do_unit_tests = false;
	const char *world_fname = NULL;

	// Parse arguments
	for(int argidx = 1; argidx < argc; argidx++)
	{
		if(!strcmp(argv[argidx], "--unit-tests"))
		{
			quit_after_tests = true;
			do_unit_tests = true;
		}
		else if(world_fname == NULL)
		{
			world_fname = argv[argidx];
		}
		else
		{
			fprintf(stderr, "unexpected argument \"%s\"\n",
				argv[argidx]);
			return 2;
		}
	}

	// If we aren't running tests and then quitting, force unit tests
	if(!quit_after_tests)
	{
		do_unit_tests = true;
	}

	if(!run_tests(quit_after_tests))
	{
		if(!quit_after_tests)
		{
			printf("# UNIT TESTS FAILED - CANNOT CONTINUE\n");
		}
		return 1;
	}

	// If we need to quit after running tests, quit here
	if(quit_after_tests)
	{
		return 0;
	}

	printf(APP_NAME " " APP_VERSION_STRING "\n");

	if(world_fname == NULL)
	{
		printf("Please provide a "
#if SUPER_ZZT
			"Super ZZT (*.SZT)"
#else /* !SUPER_ZZT */
			"ZZT (*.ZZT)"
#endif /* SUPER_ZZT */
			" world on the command line.\n");
		return 2;
	}

	IoStream *stream = io_open_binary_file_for_reading(world_fname);
	World *world = read_world(stream);
	io_close(&stream);

	if(world == NULL)
	{
		printf("ERROR: Could not load world\n");
		return 1;
	}

	// FIXME: this is a spike - split into functions for each structure in the right files!
	for(int b = 0; b < world->board_count; b++)
	{
		Board *board = world->boards[b];
		Block *block = board->block;
		printf("%s:\n", board->name.dat);
		for(int y = 0; y < block->height; y++)
		{
			for(int x = 0; x < block->width; x++)
			{
				uint8_t ch = get_block_tile_visible_char(
					block, x, y);
				uint8_t col = get_block_tile_visible_color(
					block, x, y);

				// FIXME needs to apply colour
				uint8_t outbuf[32];
				int inbuf[1];
				inbuf[0] = cp437_char_to_utf8(ch);
				write_utf8_chars_to_string(outbuf, sizeof(outbuf)-1, inbuf, 1);
				outbuf[sizeof(outbuf)-1] = 0;
				printf("\x1B[3%d;4%dm",
					((col&0x04)>>2)
					|((col&0x01)<<2)
					|(col&0x02),
					(((col>>4)&0x04)>>2)
					|(((col>>4)&0x01)<<2)
					|((col>>4)&0x02));
				if((col&0x08) != 0) { printf("\x1B[1m"); }
				if((col&0x80) != 0) { printf("\x1B[5m"); }
				printf("%s\x1B[0m", outbuf);
			}
			printf("\n");
		}
		printf("\n");
	}

	free_world(&world);

	return 0;
}

