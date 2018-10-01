//# new_block
Block *new_block(int width, int height);

//# free_block: Frees the given block if not NULL.
//#
//# block:
//#   Double-pointer to block. Pointer to block is set to NULL.
void free_block(Block **pblock);

//# get_block_tile_raw_type
tile_type get_block_tile_raw_type(Block *block, int x, int y);

//# set_block_tile_raw_type
void set_block_tile_raw_type(Block *block, int x, int y, tile_type tile);

//# get_block_tile_raw_color
uint8_t get_block_tile_raw_color(Block *block, int x, int y);

//# set_block_tile_raw_color
void set_block_tile_raw_color(Block *block, int x, int y, uint8_t tile);

//# get_block_tile_visible_char
uint8_t get_block_tile_visible_char(Block *block, int x, int y);

//# get_block_tile_visible_color
uint8_t get_block_tile_visible_color(Block *block, int x, int y);

//# add_stat_to_block
//#
//# Returns an index into the stat array of the block if successful.
//# Returns STAT_ERROR on failure.
int add_stat_to_block(Block *block, int x, int y);

