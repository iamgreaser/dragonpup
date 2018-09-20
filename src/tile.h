
typedef enum tile_type
{
#define XX(t_upname, t_intname, t_code) T_##t_upname = t_code,
#include "tile_table.gen.h"
#undef XX

} tile_type;

typedef struct tile tile;
struct tile
{
	tile_type type;
	uint8_t color;
};

//# get_tile_type_from_name
tile_type get_tile_type_from_name(const char *name);

//# get_tile_name_from_type
const char *get_tile_name_from_type(tile_type type);

