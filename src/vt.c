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

static const int cp437_to_utf8_table[256] = {
	0x0020, 0x263A, 0x263B, 0x2665, 0x2666, 0x2663, 0x2660, 0x2022,
	0x25D8, 0x25CB, 0x25D9, 0x2642, 0x2640, 0x266A, 0x266B, 0x263C,
	0x25BA, 0x25C4, 0x2195, 0x203C, 0x00B6, 0x00A7, 0x25AC, 0x21A8,
	0x2191, 0x2193, 0x2192, 0x2190, 0x221F, 0x2194, 0x25B2, 0x25BC,
	0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027,
	0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F,
	0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037,
	0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F,
	0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047,
	0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F,
	0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057,
	0x0058, 0x0059, 0x005A, 0x005B, 0x005C, 0x005D, 0x005E, 0x005F,
	0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067,
	0x0068, 0x0069, 0x006A, 0x006B, 0x006C, 0x006D, 0x006E, 0x006F,
	0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077,
	0x0078, 0x0079, 0x007A, 0x007B, 0x007C, 0x007D, 0x007E, 0x2302,
	0x0080, 0x0081, 0x0082, 0x0083, 0x00E4, 0x0085, 0x0086, 0x0087,
	0x0088, 0x0089, 0x008A, 0x00EF, 0x00EE, 0x00EC, 0x00C4, 0x00C5,
	0x0090, 0x00E6, 0x00C6, 0x0093, 0x00F6, 0x0095, 0x0096, 0x0097,
	0x00FF, 0x00D6, 0x009A, 0x00A2, 0x00A3, 0x00A5, 0x20A7, 0x0192,
	0x00A0, 0x00A1, 0x00A2, 0x00A3, 0x00A4, 0x00A5, 0x00A6, 0x00A7,
	0x00A8, 0x00A9, 0x00AA, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x00AF,
	0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x2561, 0x2562, 0x2556,
	0x2555, 0x2563, 0x2551, 0x2557, 0x255D, 0x255C, 0x255B, 0x2510,
	0x00C0, 0x00C1, 0x00C2, 0x00C3, 0x00C4, 0x253C, 0x255E, 0x00C7,
	0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x00CF,
	0x2568, 0x2564, 0x2565, 0x2559, 0x2558, 0x2552, 0x2553, 0x256B,
	0x256A, 0x2518, 0x250C, 0x2588, 0x2584, 0x258C, 0x2590, 0x2580,
	0x03B1, 0x00DF, 0x0393, 0x03C0, 0x03A3, 0x03C3, 0x00B5, 0x03C4,
	0x03A6, 0x0398, 0x03A9, 0x03B4, 0x221E, 0x03C6, 0x03B5, 0x2229,
	0x2261, 0x00B1, 0x2265, 0x2264, 0x2320, 0x2321, 0x00F7, 0x2248,
	0x00B0, 0x2219, 0x00B7, 0x221A, 0x207F, 0x00B2, 0x25A0, 0x0020,
};

int cp437_char_to_utf8(uint8_t c)
{
	return cp437_to_utf8_table[c];
}
// No tests for this - correctness is based purely on the table.

////////////////////////////////////////////////////////////////////////////

void vt_tests(void)
{
	tap_ok(true, "VT test hookup check");
	TEST_write_utf8_chars_to_string();
}
