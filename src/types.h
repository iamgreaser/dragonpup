typedef enum tile_type
{
#define XX(t_upname, t_intname, t_code, t_char, t_color, t_cycle) T_##t_upname = t_code,
#include "tile_table.gen.h"
#undef XX

} tile_type;

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
};
