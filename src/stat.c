#include "common.h"
#include "stat.h"

////////////////////////////////////////////////////////////////////////////

Stat *new_stat(int x, int y, tile_type type)
{
	Stat *stat = malloc(sizeof(Stat));

	memset(stat, 0, sizeof(stat));

	stat->x = x;
	stat->y = y;
	stat->cycle = 6;

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

	stat = new_stat(3, 2, T_BOMB);
	tap_ok(stat != NULL, "Create stat: Bomb(3,2)");
	tap_ok(stat->x == 3 && stat->y == 2, "Bomb is at (3, 2)");
	tap_ok(stat->cycle == 6, "Bomb cycle is 6");
	free_stat(&stat);
	tap_ok(true, "Stat can be freed");
	tap_ok(stat == NULL, "Stat set to NULL when freed");
	free_stat(&stat);
	tap_ok(true, "Freeing NULL stats causes no pain");
	tap_ok(stat == NULL, "NULL stat stays NULL");

}

////////////////////////////////////////////////////////////////////////////

void stat_tests(void)
{
	tap_ok(true, "Stat test hookup check");
	TEST_new_stat();
}

