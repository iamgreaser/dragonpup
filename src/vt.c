#include "common.h"
#include "vt.h"

#include <stdio.h>

////////////////////////////////////////////////////////////////////////////

size_t get_utf8_char_length(int c)
{
	if(c <= 0x10FFFF)
	{
		return 5;
	}
	else
	{
		return 0;
	}
	return 0;
}

size_t write_utf8_chars_to_string(
	uint8_t *dstbuf, size_t dstbytes,
	int *srcbuf, size_t srcelems)
{
	size_t dstidx = 0;

	for(size_t i = 0; i < srcelems; i++)
	{
		int c = srcbuf[i];

		size_t char_length = get_utf8_char_length(c);
		size_t cbeg = dstidx;
		size_t cend = dstidx + char_length;
		assert(cend <= dstbytes);

		if(c <= 0x10FFFF)
		{
			// 10FFFF: bbbbbbcc ccccdddd ddeeeeee
			//
			// 11111000
			// 10bbbbbb
			// 10cccccc
			// 10dddddd
			// 10eeeeee
			dstbuf[dstidx++] = 0xF8;
			dstbuf[dstidx++] = 0xC0 | (0x3F & (c>>18));
			dstbuf[dstidx++] = 0xC0 | (0x3F & (c>>12));
			dstbuf[dstidx++] = 0xC0 | (0x3F & (c>>6));
			dstbuf[dstidx++] = 0xC0 | (0x3F & (c));
		}
		else
		{
			// INVALID CODEPOINT
			// FIXME: cleaner error handling
			assert(!"invalid UTF-8 codepoint");
			abort();
		}

		assert(cend == dstidx);
	}

	return dstidx;
}

static void TEST_get_utf8_char_length(void)
{
	bool corelation_test_result = true;
	bool valid_points_test_result_getlen = true;
	for(int i = 0; i <= 0x10FFFF; i++)
	{
		int srcbuf[1];
		uint8_t dstbuf[8];

		srcbuf[0] = i;
		size_t expected = get_utf8_char_length(i);
		size_t actual = write_utf8_chars_to_string(
			dstbuf, sizeof(dstbuf),
			srcbuf, 1);
		
		if(expected == 0)
		{
			valid_points_test_result_getlen = false;
			fprintf(stderr, "MISINVALID: %06X\n", i);
		}

		if(expected != actual)
		{
			corelation_test_result = false;
			fprintf(stderr,
				"MISMATCH: %06X: len = %d, write = %d\n",
				i, expected, actual);
		}
	}
	tap_ok(corelation_test_result,
		"get_utf8_char_length() matches to_utf8_string()");
	tap_ok(valid_points_test_result_getlen,
		"all valid points appear in get_utf8_char_length()");

}

////////////////////////////////////////////////////////////////////////////

void vt_tests(void)
{
	tap_ok(true, "VT test hookup check");
	TEST_get_utf8_char_length();
}
