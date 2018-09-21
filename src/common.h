#ifndef SUPER_ZZT
#error "Please define SUPER_ZZT as 0 or 1!"
#endif

#include <stdbool.h>
#include <stdint.h>

#include <stdlib.h>
#include <string.h>
#include <errno.h>

// Helpers
#define SYM_TO_STR(x) #x

// Stuff everything should have
#include "types.h"

#include "tests.h"
#include "utils.h"

// All test cases go here
void board_tests(void);
void stat_tests(void);
void tile_tests(void);

#define ALL_TEST_CASES \
	tests_tests(); \
	\
	board_tests(); \
	stat_tests(); \
	tile_tests(); \
//

