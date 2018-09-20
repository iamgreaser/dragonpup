typedef enum tile_type
{
#define XX(t_upname, t_intname, t_code, t_char, t_color) T_##t_upname = t_code,
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
	int dummy; // TODO!
};

typedef struct Block Block;
struct Block
{
	uint16_t width, height;
	struct Tile *tile_data;
};
