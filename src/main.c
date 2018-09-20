#include <assert.h>
#include <stdio.h>

#include "common.h"

int main(int argc, char *argv[])
{
	const bool use_tap_output = true;

	if(!run_tests(use_tap_output))
	{
		//printf("# UNIT TESTS FAILED - CANNOT CONTINUE\n");
		return 1;
	}

	return 0;
}

