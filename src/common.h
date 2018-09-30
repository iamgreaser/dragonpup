#ifndef SUPER_ZZT
#error "Please define SUPER_ZZT as 0 or 1!"
#endif

#ifndef USE_SDL
#define USE_SDL 1
#endif

#include <stdbool.h>
#include <stdint.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#if USE_SDL
#include <SDL.h>
#endif

// Constants
#if SUPER_ZZT
#define APP_NAME "Super DragonPup"
#define ZZT_WORLD_VERSION -2
#define ZZT_BOARD_WIDTH 96
#define ZZT_BOARD_HEIGHT 80

#define ZZT_MAX_KEYS 7
#define ZZT_MAX_FLAGS 16

#else
#define APP_NAME "DragonPup"
#define ZZT_WORLD_VERSION -1
#define ZZT_BOARD_WIDTH 60
#define ZZT_BOARD_HEIGHT 25

#define ZZT_MAX_KEYS 7
#define ZZT_MAX_FLAGS 10

#endif

#define ZZT_BOARD_EXITS 4

#define APP_VERSION_STRING "vNone"

// Helpers
#define SYM_TO_STR(x) #x

// Stuff everything should have
#include "types.h"

#include "tests.h"
#include "utils.h"

// All test cases go here
void board_tests(void);
void block_tests(void);
void gfx_tests(void);
void io_tests(void);
void stat_tests(void);
void tile_tests(void);
void vt_tests(void);
void world_tests(void);

#define ALL_TEST_CASES \
	tests_tests(); \
	\
	block_tests(); \
	board_tests(); \
	gfx_tests(); \
	io_tests(); \
	stat_tests(); \
	tile_tests(); \
	vt_tests(); \
	world_tests(); \
//

