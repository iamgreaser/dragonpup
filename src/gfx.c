#include "common.h"
#include "gfx.h"

////////////////////////////////////////////////////////////////////////////

extern const uint8_t ega_font[256][14];

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 14

////////////////////////////////////////////////////////////////////////////

void blit_char_to_32bpp(void *surface,
	int width, int height, int pitch_bytes,
	int char_x, int char_y, int bg, int fg, uint8_t ch)
{
	int pixel_x = char_x * CHAR_WIDTH;
	int pixel_y = char_y * CHAR_HEIGHT;

	for(int y = 0; y < 14; y++)
	{
		uint32_t *row = (uint32_t *)(
			((uint8_t *)surface)
			+ (pitch_bytes * (pixel_y + y)));

		uint8_t chrow = ega_font[ch][y];

		for(int x = 0; x < 8; x++)
		{
			row[pixel_x + x] = ((chrow & (0x80>>x)) == 0 ? bg : fg);
		}
	}
}

static void TEST_blit_char_to_32bpp(void)
{
	uint32_t single_char_buf[14][8];

	memset(single_char_buf, 0, sizeof(single_char_buf));

	// Try some base cases
	// 0x20: space
	blit_char_to_32bpp(single_char_buf[0], 8, 14, 8*sizeof(uint32_t),
		0, 0, 0x00000000, 0xFFFFFFFF, 0x20);
	tap_ok(single_char_buf[0][0] == 0x00000000,
		"32bpp char 0x20 (bg=0, fg=1): TLC is black");
	tap_ok(single_char_buf[13][7] == 0x00000000,
		"32bpp char 0x20 (bg=0, fg=1): BRC is black");

	blit_char_to_32bpp(single_char_buf[0], 8, 14, 8*sizeof(uint32_t),
		0, 0, 0xFFFFFFFF, 0x00000000, 0x20);
	tap_ok(single_char_buf[0][0] == 0xFFFFFFFF,
		"32bpp char 0x20 (bg=1, fg=0): TLC is white");
	tap_ok(single_char_buf[13][7] == 0xFFFFFFFF,
		"32bpp char 0x20 (bg=1, fg=0): BRC is white");

	// 0xDB: solid
	blit_char_to_32bpp(single_char_buf[0], 8, 14, 8*sizeof(uint32_t),
		0, 0, 0x00000000, 0xFFFFFFFF, 0xDB);
	tap_ok(single_char_buf[0][0] == 0xFFFFFFFF,
		"32bpp char 0xDB (bg=0, fg=1): TLC is white");
	tap_ok(single_char_buf[13][7] == 0xFFFFFFFF,
		"32bpp char 0xDB (bg=0, fg=1): BRC is white");

	blit_char_to_32bpp(single_char_buf[0], 8, 14, 8*sizeof(uint32_t),
		0, 0, 0xFFFFFFFF, 0x00000000, 0xDB);
	tap_ok(single_char_buf[0][0] == 0x00000000,
		"32bpp char 0xDB (bg=1, fg=0): TLC is black");
	tap_ok(single_char_buf[0][0] == 0x00000000,
		"32bpp char 0xDB (bg=1, fg=0): BRC is black");

	// 0xDC: bottom half
	blit_char_to_32bpp(single_char_buf[0], 8, 14, 8*sizeof(uint32_t),
		0, 0, 0x00000000, 0xFFFFFFFF, 0xDC);
	tap_ok(single_char_buf[0][0] == 0x00000000,
		"32bpp char 0xDC (bg=0, fg=1): TLC is black");
	tap_ok(single_char_buf[0][7] == 0x00000000,
		"32bpp char 0xDC (bg=0, fg=1): TRC is black");
	tap_ok(single_char_buf[13][0] == 0xFFFFFFFF,
		"32bpp char 0xDC (bg=0, fg=1): BLC is white");
	tap_ok(single_char_buf[13][7] == 0xFFFFFFFF,
		"32bpp char 0xDC (bg=0, fg=1): BRC is white");

	// 0xDD: left half
	blit_char_to_32bpp(single_char_buf[0], 8, 14, 8*sizeof(uint32_t),
		0, 0, 0x00000000, 0xFFFFFFFF, 0xDD);
	tap_ok(single_char_buf[0][0] == 0xFFFFFFFF,
		"32bpp char 0xDD (bg=0, fg=1): TLC is white");
	tap_ok(single_char_buf[0][7] == 0x00000000,
		"32bpp char 0xDD (bg=0, fg=1): TRC is black");
	tap_ok(single_char_buf[13][0] == 0xFFFFFFFF,
		"32bpp char 0xDD (bg=0, fg=1): BLC is white");
	tap_ok(single_char_buf[13][7] == 0x00000000,
		"32bpp char 0xDD (bg=0, fg=1): BRC is black");
}

////////////////////////////////////////////////////////////////////////////

void gfx_tests(void)
{
	tap_ok(true, "Graphics test hookup check");
	TEST_blit_char_to_32bpp();
}

