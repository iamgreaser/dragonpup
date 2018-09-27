//# io_open_binary_file_for_reading
IoStream *io_open_binary_file_for_reading(const char *fname);

//# io_open_shared_buffer_for_reading
IoStream *io_open_shared_buffer_for_reading(const void *buf, size_t bytes);

//# io_close
void io_close(IoStream **pstream);

//# io_read
ssize_t io_read(IoStream *stream, void *buf, size_t bytes);

#define IO_PARSE_FAILURE -0x80000000

//# io_read_s16le
//# Returns IO_PARSE_FAILURE on failure.
int io_read_s16le(IoStream *stream);

//# io_read_u8
//# Returns IO_PARSE_FAILURE on failure.
int io_read_u8(IoStream *stream);

//# io_write
ssize_t io_write(IoStream *stream, void *buf, size_t bytes);
