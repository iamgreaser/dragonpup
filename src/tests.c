#include <assert.h>
#include <stdio.h>

#include "common.h"

static int tap_tests_performed = 0;
static bool tap_tests_all_ok = false;
static bool tap_tests_bailed_out = false;
static bool tap_tests_show_all_results = false;

////////////////////////////////////////////////////////////////////////////

static void tests_tests(void);

////////////////////////////////////////////////////////////////////////////

bool run_tests(bool show_all_tap_results)
{
	tap_tests_performed = 0;
	tap_tests_all_ok = true;
	tap_tests_bailed_out = false;
	tap_tests_show_all_results = show_all_tap_results;

	// TEST CASE SUITES GO HERE
	ALL_TEST_CASES

	// DONE ALL TEST CASES

	if(tap_tests_show_all_results)
	{
		printf("1..%d\n", tap_tests_performed);
	}

	return tap_tests_all_ok;
}

////////////////////////////////////////////////////////////////////////////

void tap_ok(bool is_ok, const char *test_name)
{
	int this_test_index = tap_tests_performed + 1;

	tap_tests_performed += 1;

	if(tap_tests_show_all_results || !is_ok)
	{
		printf("%s %d - %s\n",
			(is_ok ? "ok" : "not ok"),
			this_test_index,
			test_name);
	}

	if(!is_ok)
	{
		tap_tests_all_ok = false;
	}
}

////////////////////////////////////////////////////////////////////////////

static void tests_tests(void)
{
	tap_ok(true, "Test system hookup check");
}

////////////////////////////////////////////////////////////////////////////
