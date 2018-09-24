typedef enum tile_type
{
#define XX(t_upname, t_intname, t_code, t_char, t_color, t_cycle) T_##t_upname = t_code,
#include "tile_table.gen.h"
#undef XX

} tile_type;

typedef struct { uint8_t len; uint8_t dat[20+1]; } pstring20;

typedef struct Tile Tile;
struct Tile
{
	tile_type type;
	uint8_t color;
};

typedef struct Stat Stat;
struct Stat
{
	// Real structure
	uint8_t x, y;
	int16_t step_x, step_y;
	int16_t cycle;
	uint8_t p1, p2, p3;
	int16_t follower, leader;
	uint8_t under_type, under_color;
	int16_t code_pc;
	int16_t code_length;
	uint8_t *code;
};

typedef struct Block Block;
struct Block
{
	uint16_t width, height;
	struct Tile *tile_data;
};

typedef struct World World;
struct World
{
	int16_t version_magic;
	int16_t board_count;
	int16_t player_ammo;
	int16_t player_gems;
	uint8_t player_keys[ZZT_MAX_KEYS];
	int16_t player_health;
	int16_t starting_board;

#if SUPER_ZZT
	int16_t player_score;
	int16_t energy_cycles;
#else /* !SUPER_ZZT */
	int16_t player_torches;
	int16_t torch_cycles;
	int16_t energy_cycles;
	int16_t player_score;
#endif /* SUPER_ZZT */
	pstring20 world_name;
	pstring20 flags[ZZT_MAX_FLAGS];
	int16_t time_passed;
	int16_t time_passed_subticks;
	uint8_t is_saved_game;
#if SUPER_ZZT
	int16_t player_stones;
#endif /* SUPER_ZZT */
};
