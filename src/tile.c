#include "common.h"
#include "board.h"
#include "tile.h"

////////////////////////////////////////////////////////////////////////////

tile_type get_tile_type_from_name(const char *name)
{
#define XX(t_upname, t_intname, t_code, t_char, t_color, t_cycle) \
		if(match_strings_letters_only(name, t_intname)) \
		{ \
			return T_##t_upname; \
		}
#include "tile_table.gen.h"
#undef XX
	return T_MONITOR;
}

static void TEST_get_tile_type_from_name(void)
{
	// Test name lookups
	tap_ok(get_tile_type_from_name("AMMO") == T_AMMO,
		"Tile type: AMMO");
	tap_ok(get_tile_type_from_name("ammo") == T_AMMO,
		"Tile type: ammo");
	tap_ok(get_tile_type_from_name("Ammo") == T_AMMO,
		"Tile type: Ammo");
	
	// If it doesn't exist it's a monitor (TODO: confirm)
	tap_ok(get_tile_type_from_name("monitor") == T_MONITOR,
		"Invalid tile names are monitors");
	
	// If it's empty it's an edge
	tap_ok(get_tile_type_from_name("") == T_BOARD_EDGE,
		"Empty tile names are edges");
	
	// If it's not a letter in the internal name we ignore it
	tap_ok(get_tile_type_from_name("spinninggun") == T_SPINNING_GUN,
		"Tile type: Spinning gun");
	tap_ok(get_tile_type_from_name("sliderns") == T_SLIDER_NS,
		"Tile type: Slider (NS)");
	tap_ok(get_tile_type_from_name("sliderew") == T_SLIDER_EW,
		"Tile type: Slider (EW)");
}

////////////////////////////////////////////////////////////////////////////

const char *get_tile_name_from_type(tile_type type)
{
	switch(type)
	{
#define XX(t_upname, t_intname, t_code, t_char, t_color, t_cycle) \
		case T_##t_upname:\
			if(t_intname[0] == '\x00') \
			{ \
				return SYM_TO_STR(t_upname); \
			} \
			else \
			{ \
				return t_intname; \
			}
#include "tile_table.gen.h"
#undef XX
		default:
			return "??? INVALID ???";
	}
}

static void TEST_get_tile_name_from_type(void)
{
	// Test name lookups
	tap_ok(!strcasecmp("Ammo",
			get_tile_name_from_type(T_AMMO)),
		"Tile name: Ammo");

	// If it's invalid, return a sentinel string
	tap_ok(get_tile_name_from_type(-1) != NULL,
		"Invalid names don't return NULL");
	tap_ok(strlen(get_tile_name_from_type(-1)) < 100,
		"Invalid names are valid C strings and point to memory");

	// Tiles without regular internal names use a fallback
	tap_ok(!strcasecmp("Board_Edge",
			get_tile_name_from_type(T_BOARD_EDGE)),
		"Tile name: Board_Edge");
	tap_ok(!strcasecmp("Blink_Ray_H",
			get_tile_name_from_type(T_BLINK_RAY_H)),
		"Tile name: Blink_Ray_H");
}

////////////////////////////////////////////////////////////////////////////

uint8_t get_tile_char(const Tile *tile, const Stat *stat,
	const Block *block, int x, int y)
{
	// Text uses the colour for the character.
	if(tile->type >= T_TEXT_BLUE && tile->type <= T_TEXT_BLACK)
	{
		return tile->color;
	}

	// Objects grab their char from their stats if they exist.
	if(tile->type == T_OBJECT && stat != NULL)
	{
		return stat->p1;
	}

	// Default switch block coming through!
	switch(tile->type)
	{
#define XX(t_upname, t_intname, t_code, t_char, t_color, t_cycle) \
		case T_##t_upname:\
			return t_char;
#include "tile_table.gen.h"
#undef XX
		default:
			return '\x20'; // Most of these are spaces.
	}
}

static void TEST_get_tile_char(void)
{
	Tile player1 = {.type = T_PLAYER, .color = 0x1F};
	tap_ok(get_tile_char(&player1, NULL, NULL, 0, 0) == 0x02,
		"Tile char: Player");
	Tile empty1 = {.type = T_EMPTY, .color = 0x70};
	tap_ok(get_tile_char(&empty1, NULL, NULL, 0, 0) == 0x20,
		"Tile char: Empty");

	// Text is always fun.
	Tile textblue1 = {.type = T_TEXT_BLUE, .color = 0x21};
	tap_ok(get_tile_char(&textblue1, NULL, NULL, 0, 0) == 0x21,
		"Tile char: Text Blue, 0x21");
	Tile textblue2 = {.type = T_TEXT_BLUE, .color = 0x3F};
	tap_ok(get_tile_char(&textblue2, NULL, NULL, 0, 0) == 0x3F,
		"Tile char: Text Blue, 0x3F");

	// Lines need neighbour info.
	Tile line1 = {.type = T_LINE, .color = 0x0F};
	tap_ok(get_tile_char(&line1, NULL, NULL, 0, 0) == 0xCE,
		"Tile char: Line, NULL block");
}

////////////////////////////////////////////////////////////////////////////

uint8_t get_tile_color(const Tile *tile, const Stat *stat,
	const Block *block, int x, int y)
{
	// Text uses the colour for the character.
	// Thus, it has to get the colour from somewhere.
	// This tends to be the type itself.

	if(tile->type >= T_TEXT_BLUE && tile->type <= T_TEXT_BLACK)
	{
		int color = tile->type - T_TEXT_BLUE;
		color += 1;
		if(color == 7) {
			color = 0;
		}
		// TODO: handle blinking text

		// Move to BG, use white FG
		color <<= 4;
		color += 0xF;
		color &= 0xFF;
		return color;
	}

	switch(tile->type)
	{
		case T_EMPTY:
			// Empties are special. They ALWAYS have a black background.
			return 0x07;

		default:
			return tile->color;
	}
}

static void TEST_get_tile_color(void)
{
	Tile player1 = {.type = T_PLAYER, .color = 0x1F};
	tap_ok(get_tile_color(&player1, NULL, NULL, 0, 0) == 0x1F,
		"Tile color: Player");
	Tile empty1 = {.type = T_EMPTY, .color = 0x70};
	tap_ok(get_tile_color(&empty1, NULL, NULL, 0, 0) == 0x07,
		"Tile color: Empty");
	Tile lion1 = {.type = T_LION, .color = 0x0C};
	tap_ok(get_tile_color(&lion1, NULL, NULL, 0, 0) == 0x0C,
		"Tile color: Lion Red");
	Tile lion2 = {.type = T_LION, .color = 0x06};
	tap_ok(get_tile_color(&lion2, NULL, NULL, 0, 0) == 0x06,
		"Tile color: Lion Brown");

	// Text is just as always fun.
	Tile textred1 = {.type = T_TEXT_RED, .color = 0x21};
	tap_ok(get_tile_color(&textred1, NULL, NULL, 0, 0) == 0x4F,
		"Tile color: Text Red, 0x21");
	Tile textblue1 = {.type = T_TEXT_BLUE, .color = 0x21};
	tap_ok(get_tile_color(&textblue1, NULL, NULL, 0, 0) == 0x1F,
		"Tile color: Text Blue, 0x21");
	Tile textblue2 = {.type = T_TEXT_BLUE, .color = 0x3F};
	tap_ok(get_tile_color(&textblue2, NULL, NULL, 0, 0) == 0x1F,
		"Tile color: Text Blue, 0x3F");
	Tile textblack1 = {.type = T_TEXT_BLACK, .color = 0x21};
	tap_ok(get_tile_color(&textblack1, NULL, NULL, 0, 0) == 0x0F,
		"Tile color: Text Black, 0x21");
}

////////////////////////////////////////////////////////////////////////////

void tile_tests(void)
{
	tap_ok(true, "Tile test hookup check");

	TEST_get_tile_type_from_name();
	TEST_get_tile_name_from_type();
	TEST_get_tile_char();
	TEST_get_tile_color();
}

