#include "common.h"
#include "io.h"

#include <stdio.h>

struct io_stream_driver {
	ssize_t (*read)(IoStream *stream, void *buf, size_t bytes);
	ssize_t (*write)(IoStream *stream, void *buf, size_t bytes);
	void (*close)(IoStream *stream);
};

struct IoStream
{
	struct io_stream_driver *driver;
	union {
		struct {
			size_t read_offset;
			size_t bytes;
			uint8_t buf[];
		} io_buffer;

		struct {
			FILE *fp;
		} file_stream;
	} data;
};

////////////////////////////////////////////////////////////////////////////

void io_close(IoStream **pstream)
{
	if((*pstream) == NULL)
	{
		return;
	}

	assert((*pstream)->driver != NULL);
	assert((*pstream)->driver->close != NULL);

	(*pstream)->driver->close((*pstream));

	*pstream = NULL;
}

////////////////////////////////////////////////////////////////////////////

static void shared_buffer_io_close(IoStream *stream)
{
	// Nothing to do here
}

struct io_stream_driver shared_buffer_io_driver = {
	.read = NULL,
	.write = NULL,
	.close = shared_buffer_io_close,
};

IoStream *io_open_shared_buffer_for_reading(const void *buf, size_t bytes)
{
	IoStream *stream = malloc(sizeof(IoStream) + bytes);
	memset(stream, 0, sizeof(IoStream));
	stream->driver = &shared_buffer_io_driver;
	return stream;
}

static void TEST_io_open_shared_buffer_for_reading(void)
{
	IoStream *stream;

	stream = io_open_shared_buffer_for_reading(
		"Hello World!", 12);
	tap_ok(stream != NULL, "Shared buffer read I/O opened");
	io_close(&stream);
	tap_ok(stream == NULL, "Shared buffer read I/O closed");
	io_close(&stream);
	tap_ok(stream == NULL, "Shared buffer read I/O double close safe");
}

////////////////////////////////////////////////////////////////////////////

void io_tests(void)
{
	tap_ok(true, "I/O test hookup check");

	TEST_io_open_shared_buffer_for_reading();
}
