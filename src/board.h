//# new_board
Board *new_board(void);

//# free_board: Frees the given board if not NULL.
//#
//# board:
//#   Double-pointer to board. Pointer to board is set to NULL.
void free_board(Board **pboard);

//# read_board
Board *read_board(IoStream *stream);

// TEST FIXTURES
void FIXTURE_read_board(uint8_t **pbuf, size_t *pbytes);
