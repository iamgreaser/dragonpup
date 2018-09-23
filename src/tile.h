//# get_tile_type_from_name
tile_type get_tile_type_from_name(const char *name);

//# get_tile_name_from_type
const char *get_tile_name_from_type(tile_type type);

//# get_tile_char
char get_tile_char(const Tile *tile, const Stat *stat,
	const Block *block, int x, int y);

//# get_tile_color
uint8_t get_tile_color(const Tile *tile, const Stat *stat,
	const Block *block, int x, int y);
