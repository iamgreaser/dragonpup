//# io_open_binary_file_for_reading
IoStream *io_open_binary_file_for_reading(const char *fname);

//# io_open_shared_buffer_for_reading
IoStream *io_open_shared_buffer_for_reading(const void *buf, size_t bytes);

//# io_close
void io_close(IoStream **pstream);

//# io_read
ssize_t io_read(IoStream *stream, void *buf, size_t bytes);

//# io_write
ssize_t io_write(IoStream *stream, void *buf, size_t bytes);
