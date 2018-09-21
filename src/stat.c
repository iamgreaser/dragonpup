#include "common.h"
#include "stat.h"

////////////////////////////////////////////////////////////////////////////

int get_stat_default_cycle(tile_type type)
{
	switch(type)
	{
#define XX(t_upname, t_intname, t_code, t_char, t_color, t_cycle) \
		case T_##t_upname: \
			return t_cycle;
#include "tile_table.gen.h"
#undef XX
	}

	return -1;
}

////////////////////////////////////////////////////////////////////////////

Stat *new_stat(int x, int y, tile_type type)
{
	Stat *stat = malloc(sizeof(Stat));

	memset(stat, 0, sizeof(stat));

	stat->x = x;
	stat->y = y;
	stat->cycle = get_stat_default_cycle(type);

	// Step direction
	switch(type)
	{
		case T_DUPLICATOR:
		case T_BLINK_WALL:
		case T_TRANSPORTER:
		case T_PUSHER:
			// Default to north for now
			// TODO: verify
			stat->step_x = 0;
			stat->step_y = -1;
			break;
	}

	return stat;
}

void free_stat(Stat **pstat)
{
	if(*pstat != NULL)
	{
		free(*pstat);
	}

	*pstat = NULL;
}

static void TEST_new_stat(void)
{
	Stat *stat;

	// Bomb, plus free tests
	stat = new_stat(3, 2, T_BOMB);
	tap_ok(stat != NULL, "Create stat: Bomb(3, 2)");
	tap_ok(stat->x == 3 && stat->y == 2, "Bomb is at (3, 2)");
	tap_ok(stat->cycle == 6, "Bomb cycle is 6");
	tap_ok(stat->step_x == 0 && stat->step_y == 0,
		"Bomb step is idle/self");
	free_stat(&stat);
	tap_ok(true, "Stat can be freed");
	tap_ok(stat == NULL, "Stat set to NULL when freed");
	free_stat(&stat);
	tap_ok(true, "Freeing NULL stats causes no pain");
	tap_ok(stat == NULL, "NULL stat stays NULL");

	// Duplicator
	stat = new_stat(4, 3, T_DUPLICATOR);
	tap_ok(stat != NULL, "Create stat: Duplicator(4, 3)");
	tap_ok(stat->x == 4 && stat->y == 3, "Duplicator is at (4, 3)");
	tap_ok(stat->cycle == 2, "Duplicator cycle is 2");
	// TODO!
	tap_ok(stat->step_x == 0 && stat->step_y == -1,
		"Duplicator step is north");
	free_stat(&stat);

}

////////////////////////////////////////////////////////////////////////////

void stat_tests(void)
{
	tap_ok(true, "Stat test hookup check");
	TEST_new_stat();
}

