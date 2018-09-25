//# new_board
Board *new_board(void);

//# free_board: Frees the given board if not NULL.
//#
//# board:
//#   Double-pointer to board. Pointer to board is set to NULL.
void free_board(Board **pboard);

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
