#include "common.h"
#include "vt.h"

#include <stdio.h>

////////////////////////////////////////////////////////////////////////////

size_t get_utf8_char_length(int c)
{
	if(c <= 0x7F)
	{
		return 1;
	}
	else if(c <= 0x7FF)
	{
		return 2;
	}
	else if(c <= 0xFFFF)
	{
		return 3;
	}
	else if(c <= 0x10FFFF)
	{
		return 4;
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

		// Cut it short if we have to.
		if(cend > dstbytes)
		{
			break;
		}

		assert(cend <= dstbytes);

		if(c <= 0x7F)
		{
			// 7F: 0aaaaaaa
			dstbuf[dstidx++] = c;
		}
		else if(c <= 0x7FF)
		{
			// 7FF: 110aaaaa 10bbbbbb
			dstbuf[dstidx++] = 0xC0 | (0x1F & (c>>6));
			dstbuf[dstidx++] = 0x80 | (0x3F & (c));
		}
		else if(c <= 0xFFFF)
		{
			// FFFF: 1110aaaa 10bbbbbb 10cccccc
			dstbuf[dstidx++] = 0xE0 | (0x0F & (c>>12));
			dstbuf[dstidx++] = 0x80 | (0x3F & (c>>6));
			dstbuf[dstidx++] = 0x80 | (0x3F & (c));
		}
		else if(c <= 0x10FFFF)
		{
			// 10FFFF: 11110aaa 10bbbbbb 10cccccc 10dddddd
			dstbuf[dstidx++] = 0xF0 | (0x07 & (c>>18));
			dstbuf[dstidx++] = 0x80 | (0x3F & (c>>12));
			dstbuf[dstidx++] = 0x80 | (0x3F & (c>>6));
			dstbuf[dstidx++] = 0x80 | (0x3F & (c));
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

	// Add trailing nul iff it can fit
	if(dstidx < dstbytes)
	{
		dstbuf[dstidx] = 0;
	}

	return dstidx;
}

static void TEST_write_utf8_chars_to_string(void)
{
	bool corelation_test_result = true;
	bool valid_points_test_result_getlen = true;

	// Exhaustive search
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
		"get_utf8_char_length() matches write_utf8_chars_to_string()");
	tap_ok(valid_points_test_result_getlen,
		"all valid points appear in get_utf8_char_length()");

	// Specific cases
	tap_ok(get_utf8_char_length(0x7F) == 1,
		"0x7F should be length 1");
	tap_ok(get_utf8_char_length(0x80) == 2,
		"0x80 should be length 2");
	tap_ok(get_utf8_char_length(0x3C0) == 2,
		"0x3C0 (lower pi) should be length 2");
	tap_ok(get_utf8_char_length(0x7FF) == 2,
		"0x7FF should be length 2");
	tap_ok(get_utf8_char_length(0x800) == 3,
		"0x800 should be length 3");
	tap_ok(get_utf8_char_length(0xFFFF) == 3,
		"0xFFFF should be length 3");
	tap_ok(get_utf8_char_length(0x10000) == 4,
		"0x10000 should be length 4");
	tap_ok(get_utf8_char_length(0x10FFFF) == 4,
		"0x10FFFF should be length 4");

	// String run
	{
		const char *ref_output = "\xcf\x80_\xe2\x80\xbd";
		int srcbuf[3] = {0x03C0, '_', 0x203D};
		uint8_t dstbuf[8];
		size_t bytes_written;
		memset(dstbuf, 1, 8);
		bytes_written = write_utf8_chars_to_string(
			dstbuf, 8, srcbuf, 3);
		tap_ok(!memcmp(dstbuf, ref_output, 7),
			"UTF string should add nul w/ 2 spare bytes");
		tap_ok(bytes_written == 6,
			"UTF string length of buffer size 8");

		memset(dstbuf, 1, 8);
		bytes_written = write_utf8_chars_to_string(
			dstbuf, 7, srcbuf, 3);
		tap_ok(!memcmp(dstbuf, ref_output, 7),
			"UTF string should add nul w/ 1 spare byte");
		tap_ok(bytes_written == 6,
			"UTF string length of buffer size 8");

		memset(dstbuf, 1, 8);
		bytes_written = write_utf8_chars_to_string(
			dstbuf, 6, srcbuf, 3);
		tap_ok((!memcmp(dstbuf, ref_output, 6))
				&& dstbuf[6] == 1,
			"UTF string should not add nul w/ 0 spares");
		tap_ok(bytes_written == 6,
			"UTF string length of buffer size 6");

		memset(dstbuf, 1, 8);
		bytes_written = write_utf8_chars_to_string(
			dstbuf, 5, srcbuf, 3);
		tap_ok((!memcmp(dstbuf, ref_output, 3))
				&& dstbuf[3] == 0
				&& dstbuf[4] == 1
				&& dstbuf[5] == 1
				&& dstbuf[6] == 1,
			"UTF string should truncate w/ 1 whole symbol short");
		tap_ok(bytes_written == 3,
			"UTF string length of buffer size 5 w/ truncation");
	}
}

////////////////////////////////////////////////////////////////////////////

void vt_tests(void)
{
	tap_ok(true, "VT test hookup check");
	TEST_write_utf8_chars_to_string();
}
