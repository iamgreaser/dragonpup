//# new_world
World *new_world(void);

//# free_world: Frees the given world if not NULL.
//#
//# world:
//#   Double-pointer to world. Pointer to world is set to NULL.
void free_world(World **pworld);

//# read_world
World *read_world(IoStream *stream);
