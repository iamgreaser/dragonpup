#include <assert.h>
#include <stdio.h>

#include "common.h"

int main(int argc, char *argv[])
{
	bool quit_after_tests = false;
	bool do_unit_tests = false;

	// Parse arguments
	for(int argidx = 1; argidx < argc; argidx++)
	{
		if(!strcmp(argv[argidx], "--unit-tests"))
		{
			quit_after_tests = true;
			do_unit_tests = true;
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

	return 0;
}

