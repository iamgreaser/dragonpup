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

