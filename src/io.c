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

ssize_t io_read(IoStream *stream, void *buf, size_t bytes)
{
	if(stream == NULL)
	{
		assert(!"TODO!");
		return 0; // TODO: return I/O error
	}

	assert(stream->driver != NULL);
	assert(stream->driver->read != NULL); // TODO: handle write-only streams gracefully?

	return stream->driver->read(stream, buf, bytes);
}

////////////////////////////////////////////////////////////////////////////

static ssize_t shared_buffer_io_read(IoStream *stream, void *buf, size_t bytes)
{
	size_t src_start = stream->data.io_buffer.read_offset;
	size_t src_end = src_start + bytes;

	if(src_end >= stream->data.io_buffer.bytes) {
		src_end = stream->data.io_buffer.bytes;
	}

	size_t out_bytes = src_end - src_start;
	assert(out_bytes <= bytes);

	if(out_bytes != 0)
	{
		memcpy(buf,
			stream->data.io_buffer.buf + src_start,
			out_bytes);
	}

	stream->data.io_buffer.read_offset = src_end;

	return out_bytes;
}

static void shared_buffer_io_close(IoStream *stream)
{
	// Nothing to do here
}

struct io_stream_driver shared_buffer_io_driver = {
	.read = shared_buffer_io_read,
	.write = NULL,
	.close = shared_buffer_io_close,
};

IoStream *io_open_shared_buffer_for_reading(const void *buf, size_t bytes)
{
	IoStream *stream = malloc(sizeof(IoStream) + bytes);
	memset(stream, 0, sizeof(IoStream));
	stream->driver = &shared_buffer_io_driver;
	stream->data.io_buffer.bytes = bytes;
	memcpy(stream->data.io_buffer.buf, buf, bytes);

	return stream;
}

static void TEST_io_open_shared_buffer_for_reading(void)
{
	IoStream *stream;

	stream = io_open_shared_buffer_for_reading(
		"Hello World!", 12);
	tap_ok(stream != NULL, "Shared buffer read I/O opened");

	{
		uint8_t hello_buf[5];
		ssize_t hello_bytes = io_read(stream, hello_buf, sizeof(hello_buf));
		tap_ok(hello_bytes == 5, "Read 5 bytes");
		tap_ok(!memcmp(hello_buf, "Hello", 5),
			"Contents are \"Hello\"");
		uint8_t world_buf[8];
		world_buf[7] = '1';
		ssize_t world_bytes = io_read(stream, world_buf, sizeof(world_buf));
		tap_ok(world_bytes == 7, "Read 7 bytes from 8");
		tap_ok(!memcmp(world_buf, " World!", 7),
			"Contents are \" World!\"");
		tap_ok(world_buf[7] == '1',
			"Buffer not overrun");
		uint8_t stub_buf[8] = "STUB!!!1";
		ssize_t stub_bytes = io_read(stream, stub_buf, sizeof(stub_buf));
		tap_ok(stub_bytes == 0, "Read 0 bytes from 8 at EOF");
		tap_ok(!memcmp(stub_buf, "STUB!!!1", 8),
			"Buffer unaffected");
	}

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
