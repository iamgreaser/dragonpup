#include <assert.h>
#include <stdio.h>

#include "common.h"
#include "io.h"
#include "world.h"

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

	free_world(&world);

	return 0;
}

