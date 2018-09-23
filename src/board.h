//# new_block
Block *new_block(int width, int height);

//# free_block: Frees the given block if not NULL.
//#
//# block:
//#   Double-pointer to block. Pointer to block is set to NULL.
void free_block(Block **pblock);


