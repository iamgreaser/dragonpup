//# new_stat
Stat *new_stat(int x, int y, tile_type type);

//# free_stat: Frees the given stat if not NULL.
//#
//# stat:
//#   Double-pointer to stat. Pointer to stat is set to NULL.
void free_stat(Stat **pstat);
