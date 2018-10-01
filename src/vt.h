
//# write_utf8_chars_to_string
//
// Returns number of bytes written to dstbuf.
size_t write_utf8_chars_to_string(
	uint8_t *dstbuf, size_t dstbytes,
	int *srcbuf, size_t srcelems);

//# cp437_char_to_utf8
int cp437_char_to_utf8(uint8_t c);
