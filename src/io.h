//# io_open_binary_file_for_reading
IoStream *io_open_binary_file_for_reading(const char *fname);

//# io_open_shared_buffer_for_reading
IoStream *io_open_shared_buffer_for_reading(const void *buf, size_t bytes);

//# io_close_file
void io_close_file(IoStream **pstream);

//# io_read_file
ssize_t io_read_file(IoStream *stream, void *buf, size_t bytes);

//# io_write_file
ssize_t io_write_file(IoStream *stream, void *buf, size_t bytes);
