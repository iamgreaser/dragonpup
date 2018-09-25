#include "common.h"
#include "io.h"

#include <stdio.h>

struct IoStream
{
	struct {
		ssize_t (*read)(IoStream *stream, void *buf, size_t bytes);
		ssize_t (*write)(IoStream *stream, void *buf, size_t bytes);
		void (*close)(IoStream *stream);
	} driver;
	union {
		struct {
			size_t bytes;
			uint8_t buf[];
		} io_buffer;

		struct {
			FILE *fp;
		} file_stream;
	} data;
};

////////////////////////////////////////////////////////////////////////////

void io_tests(void)
{
	tap_ok(true, "I/O test hookup check");
}
