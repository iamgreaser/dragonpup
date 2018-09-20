#include "common.h"
#include "tile.h"

////////////////////////////////////////////////////////////////////////////

tile_type get_tile_type_from_name(const char *name)
{
#define XX(t_upname, t_intname, t_code) \
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
#define XX(t_upname, t_intname, t_code) \
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

void tile_tests(void)
{
	tap_ok(true, "Tile test hookup check");

	TEST_get_tile_type_from_name();
	TEST_get_tile_name_from_type();
}

