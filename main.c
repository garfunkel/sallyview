#include <MagickWand/MagickWand.h>
#include <stdio.h>

#define GLYPH_BLOCK 0
#define GLYPH_LEFT_1_4 1
#define GLYPH_LEFT_2_4 2
#define GLYPH_LEFT_3_4 3
#define GLYPH_BOTTOM_1_8 4
#define GLYPH_BOTTOM_2_8 5
#define GLYPH_BOTTOM_3_8 6
#define GLYPH_BOTTOM_4_8 7
#define GLYPH_BOTTOM_5_8 8
#define GLYPH_BOTTOM_6_8 9
#define GLYPH_BOTTOM_7_8 10
#define GLYPH_QUAD_TOP_LEFT 11
#define GLYPH_QUAD_TOP_RIGHT 12
#define GLYPH_QUAD_BOTTOM_LEFT 13
#define GLYPH_QUAD_BOTTOM_RIGHT 14
#define GLYPH_QUAD_TOP_LEFT_BOTTOM_RIGHT 15

#define GLYPH_BLOCK_STR "\u2588"
#define GLYPH_LEFT_1_4_STR "\u258E"
#define GLYPH_LEFT_2_4_STR "\u258C"
#define GLYPH_LEFT_3_4_STR "\u258A"
#define GLYPH_BOTTOM_1_8_STR "\u2581"
#define GLYPH_BOTTOM_2_8_STR "\u2582"
#define GLYPH_BOTTOM_3_8_STR "\u2583"
#define GLYPH_BOTTOM_4_8_STR "\u2584"
#define GLYPH_BOTTOM_5_8_STR "\u2585"
#define GLYPH_BOTTOM_6_8_STR "\u2586"
#define GLYPH_BOTTOM_7_8_STR "\u2587"
#define GLYPH_QUAD_TOP_LEFT_STR "\u2598"
#define GLYPH_QUAD_TOP_RIGHT_STR "\u259D"
#define GLYPH_QUAD_BOTTOM_LEFT_STR "\u2596"
#define GLYPH_QUAD_BOTTOM_RIGHT_STR "\u2597"
#define GLYPH_QUAD_TOP_LEFT_BOTTOM_RIGHT_STR "\u259A"

#define NUM_DIFFERENT_GLYPHS 16
#define GlyphIndex int

#define diff(a, b) a >= b ? a - b : b - a

typedef struct {
	unsigned int fg_red;
	unsigned int fg_green;
	unsigned int fg_blue;
	unsigned int bg_red;
	unsigned int bg_green;
	unsigned int bg_blue;
} Glyph;

typedef Glyph Block[NUM_DIFFERENT_GLYPHS];

const char *getBestGlyph(Block *block, GlyphIndex *best_glyph) {
	unsigned int largest_contrast = 0;

	for (size_t index = 0; index < NUM_DIFFERENT_GLYPHS; index++) {
		Glyph glyph = (*block)[index];

		unsigned int contrast = diff(glyph.fg_red, glyph.bg_red);
		contrast += diff(glyph.fg_green, glyph.bg_green);
		contrast += diff(glyph.fg_blue, glyph.bg_blue);

		if (contrast > largest_contrast) {
			largest_contrast = contrast;
			*best_glyph = index;
		}
	}

	if (*best_glyph == GLYPH_LEFT_1_4) {
		return GLYPH_LEFT_1_4_STR;
	} else if (*best_glyph == GLYPH_LEFT_2_4) {
		return GLYPH_LEFT_2_4_STR;
	} else if (*best_glyph == GLYPH_LEFT_3_4) {
		return GLYPH_LEFT_3_4_STR;
	} else if (*best_glyph == GLYPH_BOTTOM_1_8) {
		return GLYPH_BOTTOM_1_8_STR;
	} else if (*best_glyph == GLYPH_BOTTOM_2_8) {
		return GLYPH_BOTTOM_2_8_STR;
	} else if (*best_glyph == GLYPH_BOTTOM_3_8) {
		return GLYPH_BOTTOM_3_8_STR;
	} else if (*best_glyph == GLYPH_BOTTOM_4_8) {
		return GLYPH_BOTTOM_4_8_STR;
	} else if (*best_glyph == GLYPH_BOTTOM_5_8) {
		return GLYPH_BOTTOM_5_8_STR;
	} else if (*best_glyph == GLYPH_BOTTOM_6_8) {
		return GLYPH_BOTTOM_6_8_STR;
	} else if (*best_glyph == GLYPH_BOTTOM_7_8) {
		return GLYPH_BOTTOM_7_8_STR;
	} else if (*best_glyph == GLYPH_QUAD_TOP_LEFT) {
		return GLYPH_QUAD_TOP_LEFT_STR;
	} else if (*best_glyph == GLYPH_QUAD_TOP_RIGHT) {
		return GLYPH_QUAD_TOP_RIGHT_STR;
	} else if (*best_glyph == GLYPH_QUAD_BOTTOM_LEFT) {
		return GLYPH_QUAD_BOTTOM_LEFT_STR;
	} else if (*best_glyph == GLYPH_QUAD_BOTTOM_RIGHT) {
		return GLYPH_QUAD_BOTTOM_RIGHT_STR;
	} else if (*best_glyph == GLYPH_QUAD_TOP_LEFT_BOTTOM_RIGHT) {
		return GLYPH_QUAD_TOP_LEFT_BOTTOM_RIGHT_STR;
	}

	return GLYPH_BLOCK_STR;
}

int main(/*int argc, char **argv*/) {
	MagickWandGenesis();

	MagickWand *wand = NewMagickWand();
	MagickBooleanType status = MagickReadImage(wand, "/Users/simon/wd.jpg");

	if (status == MagickFalse) {
		// ThrowException(wand);
	}

	size_t width = MagickGetImageWidth(wand);
	size_t height = MagickGetImageHeight(wand);
	size_t num_cols = width / 4;
	size_t num_rows = height / 8;

	for (size_t row_index = 0; row_index < num_rows; row_index++) {
		size_t y = row_index * 8;

		for (size_t col_index = 0; col_index < num_cols; col_index++) {
			size_t x = col_index * 4;
			PixelIterator *iter = NewPixelRegionIterator(wand, x, y, 4, 8);
			PixelWand **row = NULL;
			size_t num_wands = 0;
			size_t block_y = 0;
			Block block = {0};

			while ((row = PixelGetNextIteratorRow(iter, &num_wands)) != NULL) {
				for (size_t block_x = 0; block_x < num_wands; block_x++) {
					unsigned int red = 255 * PixelGetRed(row[block_x]);
					unsigned int green = 255 * PixelGetGreen(row[block_x]);
					unsigned int blue = 255 * PixelGetBlue(row[block_x]);

					block[GLYPH_BLOCK].fg_red += red;
					block[GLYPH_BLOCK].fg_green += green;
					block[GLYPH_BLOCK].fg_blue += blue;
					block[GLYPH_BLOCK].bg_red += red;
					block[GLYPH_BLOCK].bg_green += green;
					block[GLYPH_BLOCK].bg_blue += blue;

					if (block_x == 0) {
						block[GLYPH_LEFT_1_4].fg_red += red;
						block[GLYPH_LEFT_1_4].fg_green += green;
						block[GLYPH_LEFT_1_4].fg_blue += blue;

						block[GLYPH_LEFT_2_4].fg_red += red;
						block[GLYPH_LEFT_2_4].fg_green += green;
						block[GLYPH_LEFT_2_4].fg_blue += blue;

						block[GLYPH_LEFT_3_4].fg_red += red;
						block[GLYPH_LEFT_3_4].fg_green += green;
						block[GLYPH_LEFT_3_4].fg_blue += blue;
					} else if (block_x == 1) {
						block[GLYPH_LEFT_1_4].bg_red += red;
						block[GLYPH_LEFT_1_4].bg_green += green;
						block[GLYPH_LEFT_1_4].bg_blue += blue;

						block[GLYPH_LEFT_2_4].fg_red += red;
						block[GLYPH_LEFT_2_4].fg_green += green;
						block[GLYPH_LEFT_2_4].fg_blue += blue;

						block[GLYPH_LEFT_3_4].fg_red += red;
						block[GLYPH_LEFT_3_4].fg_green += green;
						block[GLYPH_LEFT_3_4].fg_blue += blue;
					} else if (block_x == 2) {
						block[GLYPH_LEFT_1_4].bg_red += red;
						block[GLYPH_LEFT_1_4].bg_green += green;
						block[GLYPH_LEFT_1_4].bg_blue += blue;

						block[GLYPH_LEFT_2_4].bg_red += red;
						block[GLYPH_LEFT_2_4].bg_green += green;
						block[GLYPH_LEFT_2_4].bg_blue += blue;

						block[GLYPH_LEFT_3_4].fg_red += red;
						block[GLYPH_LEFT_3_4].fg_green += green;
						block[GLYPH_LEFT_3_4].fg_blue += blue;
					} else if (block_x == 3) {
						block[GLYPH_LEFT_1_4].bg_red += red;
						block[GLYPH_LEFT_1_4].bg_green += green;
						block[GLYPH_LEFT_1_4].bg_blue += blue;

						block[GLYPH_LEFT_2_4].bg_red += red;
						block[GLYPH_LEFT_2_4].bg_green += green;
						block[GLYPH_LEFT_2_4].bg_blue += blue;

						block[GLYPH_LEFT_3_4].bg_red += red;
						block[GLYPH_LEFT_3_4].bg_green += green;
						block[GLYPH_LEFT_3_4].bg_blue += blue;
					}

					if (block_y == 0) {
						block[GLYPH_BOTTOM_1_8].bg_red += red;
						block[GLYPH_BOTTOM_1_8].bg_green += green;
						block[GLYPH_BOTTOM_1_8].bg_blue += blue;

						block[GLYPH_BOTTOM_2_8].bg_red += red;
						block[GLYPH_BOTTOM_2_8].bg_green += green;
						block[GLYPH_BOTTOM_2_8].bg_blue += blue;

						block[GLYPH_BOTTOM_3_8].bg_red += red;
						block[GLYPH_BOTTOM_3_8].bg_green += green;
						block[GLYPH_BOTTOM_3_8].bg_blue += blue;

						block[GLYPH_BOTTOM_4_8].bg_red += red;
						block[GLYPH_BOTTOM_4_8].bg_green += green;
						block[GLYPH_BOTTOM_4_8].bg_blue += blue;

						block[GLYPH_BOTTOM_5_8].bg_red += red;
						block[GLYPH_BOTTOM_5_8].bg_green += green;
						block[GLYPH_BOTTOM_5_8].bg_blue += blue;

						block[GLYPH_BOTTOM_6_8].bg_red += red;
						block[GLYPH_BOTTOM_6_8].bg_green += green;
						block[GLYPH_BOTTOM_6_8].bg_blue += blue;

						block[GLYPH_BOTTOM_7_8].bg_red += red;
						block[GLYPH_BOTTOM_7_8].bg_green += green;
						block[GLYPH_BOTTOM_7_8].bg_blue += blue;
					} else if (block_y == 1) {
						block[GLYPH_BOTTOM_1_8].bg_red += red;
						block[GLYPH_BOTTOM_1_8].bg_green += green;
						block[GLYPH_BOTTOM_1_8].bg_blue += blue;

						block[GLYPH_BOTTOM_2_8].bg_red += red;
						block[GLYPH_BOTTOM_2_8].bg_green += green;
						block[GLYPH_BOTTOM_2_8].bg_blue += blue;

						block[GLYPH_BOTTOM_3_8].bg_red += red;
						block[GLYPH_BOTTOM_3_8].bg_green += green;
						block[GLYPH_BOTTOM_3_8].bg_blue += blue;

						block[GLYPH_BOTTOM_4_8].bg_red += red;
						block[GLYPH_BOTTOM_4_8].bg_green += green;
						block[GLYPH_BOTTOM_4_8].bg_blue += blue;

						block[GLYPH_BOTTOM_5_8].bg_red += red;
						block[GLYPH_BOTTOM_5_8].bg_green += green;
						block[GLYPH_BOTTOM_5_8].bg_blue += blue;

						block[GLYPH_BOTTOM_6_8].bg_red += red;
						block[GLYPH_BOTTOM_6_8].bg_green += green;
						block[GLYPH_BOTTOM_6_8].bg_blue += blue;

						block[GLYPH_BOTTOM_7_8].fg_red += red;
						block[GLYPH_BOTTOM_7_8].fg_green += green;
						block[GLYPH_BOTTOM_7_8].fg_blue += blue;
					} else if (block_y == 2) {
						block[GLYPH_BOTTOM_1_8].bg_red += red;
						block[GLYPH_BOTTOM_1_8].bg_green += green;
						block[GLYPH_BOTTOM_1_8].bg_blue += blue;

						block[GLYPH_BOTTOM_2_8].bg_red += red;
						block[GLYPH_BOTTOM_2_8].bg_green += green;
						block[GLYPH_BOTTOM_2_8].bg_blue += blue;

						block[GLYPH_BOTTOM_3_8].bg_red += red;
						block[GLYPH_BOTTOM_3_8].bg_green += green;
						block[GLYPH_BOTTOM_3_8].bg_blue += blue;

						block[GLYPH_BOTTOM_4_8].bg_red += red;
						block[GLYPH_BOTTOM_4_8].bg_green += green;
						block[GLYPH_BOTTOM_4_8].bg_blue += blue;

						block[GLYPH_BOTTOM_5_8].bg_red += red;
						block[GLYPH_BOTTOM_5_8].bg_green += green;
						block[GLYPH_BOTTOM_5_8].bg_blue += blue;

						block[GLYPH_BOTTOM_6_8].fg_red += red;
						block[GLYPH_BOTTOM_6_8].fg_green += green;
						block[GLYPH_BOTTOM_6_8].fg_blue += blue;

						block[GLYPH_BOTTOM_7_8].fg_red += red;
						block[GLYPH_BOTTOM_7_8].fg_green += green;
						block[GLYPH_BOTTOM_7_8].fg_blue += blue;
					} else if (block_y == 3) {
						block[GLYPH_BOTTOM_1_8].bg_red += red;
						block[GLYPH_BOTTOM_1_8].bg_green += green;
						block[GLYPH_BOTTOM_1_8].bg_blue += blue;

						block[GLYPH_BOTTOM_2_8].bg_red += red;
						block[GLYPH_BOTTOM_2_8].bg_green += green;
						block[GLYPH_BOTTOM_2_8].bg_blue += blue;

						block[GLYPH_BOTTOM_3_8].bg_red += red;
						block[GLYPH_BOTTOM_3_8].bg_green += green;
						block[GLYPH_BOTTOM_3_8].bg_blue += blue;

						block[GLYPH_BOTTOM_4_8].bg_red += red;
						block[GLYPH_BOTTOM_4_8].bg_green += green;
						block[GLYPH_BOTTOM_4_8].bg_blue += blue;

						block[GLYPH_BOTTOM_5_8].fg_red += red;
						block[GLYPH_BOTTOM_5_8].fg_green += green;
						block[GLYPH_BOTTOM_5_8].fg_blue += blue;

						block[GLYPH_BOTTOM_6_8].fg_red += red;
						block[GLYPH_BOTTOM_6_8].fg_green += green;
						block[GLYPH_BOTTOM_6_8].fg_blue += blue;

						block[GLYPH_BOTTOM_7_8].fg_red += red;
						block[GLYPH_BOTTOM_7_8].fg_green += green;
						block[GLYPH_BOTTOM_7_8].fg_blue += blue;
					} else if (block_y == 4) {
						block[GLYPH_BOTTOM_1_8].bg_red += red;
						block[GLYPH_BOTTOM_1_8].bg_green += green;
						block[GLYPH_BOTTOM_1_8].bg_blue += blue;

						block[GLYPH_BOTTOM_2_8].bg_red += red;
						block[GLYPH_BOTTOM_2_8].bg_green += green;
						block[GLYPH_BOTTOM_2_8].bg_blue += blue;

						block[GLYPH_BOTTOM_3_8].bg_red += red;
						block[GLYPH_BOTTOM_3_8].bg_green += green;
						block[GLYPH_BOTTOM_3_8].bg_blue += blue;

						block[GLYPH_BOTTOM_4_8].fg_red += red;
						block[GLYPH_BOTTOM_4_8].fg_green += green;
						block[GLYPH_BOTTOM_4_8].fg_blue += blue;

						block[GLYPH_BOTTOM_5_8].fg_red += red;
						block[GLYPH_BOTTOM_5_8].fg_green += green;
						block[GLYPH_BOTTOM_5_8].fg_blue += blue;

						block[GLYPH_BOTTOM_6_8].fg_red += red;
						block[GLYPH_BOTTOM_6_8].fg_green += green;
						block[GLYPH_BOTTOM_6_8].fg_blue += blue;

						block[GLYPH_BOTTOM_7_8].fg_red += red;
						block[GLYPH_BOTTOM_7_8].fg_green += green;
						block[GLYPH_BOTTOM_7_8].fg_blue += blue;
					} else if (block_y == 5) {
						block[GLYPH_BOTTOM_1_8].bg_red += red;
						block[GLYPH_BOTTOM_1_8].bg_green += green;
						block[GLYPH_BOTTOM_1_8].bg_blue += blue;

						block[GLYPH_BOTTOM_2_8].bg_red += red;
						block[GLYPH_BOTTOM_2_8].bg_green += green;
						block[GLYPH_BOTTOM_2_8].bg_blue += blue;

						block[GLYPH_BOTTOM_3_8].fg_red += red;
						block[GLYPH_BOTTOM_3_8].fg_green += green;
						block[GLYPH_BOTTOM_3_8].fg_blue += blue;

						block[GLYPH_BOTTOM_4_8].fg_red += red;
						block[GLYPH_BOTTOM_4_8].fg_green += green;
						block[GLYPH_BOTTOM_4_8].fg_blue += blue;

						block[GLYPH_BOTTOM_5_8].fg_red += red;
						block[GLYPH_BOTTOM_5_8].fg_green += green;
						block[GLYPH_BOTTOM_5_8].fg_blue += blue;

						block[GLYPH_BOTTOM_6_8].fg_red += red;
						block[GLYPH_BOTTOM_6_8].fg_green += green;
						block[GLYPH_BOTTOM_6_8].fg_blue += blue;

						block[GLYPH_BOTTOM_7_8].fg_red += red;
						block[GLYPH_BOTTOM_7_8].fg_green += green;
						block[GLYPH_BOTTOM_7_8].fg_blue += blue;
					} else if (block_y == 6) {
						block[GLYPH_BOTTOM_1_8].bg_red += red;
						block[GLYPH_BOTTOM_1_8].bg_green += green;
						block[GLYPH_BOTTOM_1_8].bg_blue += blue;

						block[GLYPH_BOTTOM_2_8].fg_red += red;
						block[GLYPH_BOTTOM_2_8].fg_green += green;
						block[GLYPH_BOTTOM_2_8].fg_blue += blue;

						block[GLYPH_BOTTOM_3_8].fg_red += red;
						block[GLYPH_BOTTOM_3_8].fg_green += green;
						block[GLYPH_BOTTOM_3_8].fg_blue += blue;

						block[GLYPH_BOTTOM_4_8].fg_red += red;
						block[GLYPH_BOTTOM_4_8].fg_green += green;
						block[GLYPH_BOTTOM_4_8].fg_blue += blue;

						block[GLYPH_BOTTOM_5_8].fg_red += red;
						block[GLYPH_BOTTOM_5_8].fg_green += green;
						block[GLYPH_BOTTOM_5_8].fg_blue += blue;

						block[GLYPH_BOTTOM_6_8].fg_red += red;
						block[GLYPH_BOTTOM_6_8].fg_green += green;
						block[GLYPH_BOTTOM_6_8].fg_blue += blue;

						block[GLYPH_BOTTOM_7_8].fg_red += red;
						block[GLYPH_BOTTOM_7_8].fg_green += green;
						block[GLYPH_BOTTOM_7_8].fg_blue += blue;
					} else if (block_y == 7) {
						block[GLYPH_BOTTOM_1_8].fg_red += red;
						block[GLYPH_BOTTOM_1_8].fg_green += green;
						block[GLYPH_BOTTOM_1_8].fg_blue += blue;

						block[GLYPH_BOTTOM_2_8].fg_red += red;
						block[GLYPH_BOTTOM_2_8].fg_green += green;
						block[GLYPH_BOTTOM_2_8].fg_blue += blue;

						block[GLYPH_BOTTOM_3_8].fg_red += red;
						block[GLYPH_BOTTOM_3_8].fg_green += green;
						block[GLYPH_BOTTOM_3_8].fg_blue += blue;

						block[GLYPH_BOTTOM_4_8].fg_red += red;
						block[GLYPH_BOTTOM_4_8].fg_green += green;
						block[GLYPH_BOTTOM_4_8].fg_blue += blue;

						block[GLYPH_BOTTOM_5_8].fg_red += red;
						block[GLYPH_BOTTOM_5_8].fg_green += green;
						block[GLYPH_BOTTOM_5_8].fg_blue += blue;

						block[GLYPH_BOTTOM_6_8].fg_red += red;
						block[GLYPH_BOTTOM_6_8].fg_green += green;
						block[GLYPH_BOTTOM_6_8].fg_blue += blue;

						block[GLYPH_BOTTOM_7_8].fg_red += red;
						block[GLYPH_BOTTOM_7_8].fg_green += green;
						block[GLYPH_BOTTOM_7_8].fg_blue += blue;
					}

					if (block_x < 2 && block_y < 4) {
						block[GLYPH_QUAD_TOP_LEFT].fg_red += red;
						block[GLYPH_QUAD_TOP_LEFT].fg_green += green;
						block[GLYPH_QUAD_TOP_LEFT].fg_blue += blue;

						block[GLYPH_QUAD_TOP_RIGHT].bg_red += red;
						block[GLYPH_QUAD_TOP_RIGHT].bg_green += green;
						block[GLYPH_QUAD_TOP_RIGHT].bg_blue += blue;

						block[GLYPH_QUAD_BOTTOM_LEFT].bg_red += red;
						block[GLYPH_QUAD_BOTTOM_LEFT].bg_green += green;
						block[GLYPH_QUAD_BOTTOM_LEFT].bg_blue += blue;

						block[GLYPH_QUAD_BOTTOM_RIGHT].bg_red += red;
						block[GLYPH_QUAD_BOTTOM_RIGHT].bg_green += green;
						block[GLYPH_QUAD_BOTTOM_RIGHT].bg_blue += blue;

						block[GLYPH_QUAD_TOP_LEFT_BOTTOM_RIGHT].fg_red += red;
						block[GLYPH_QUAD_TOP_LEFT_BOTTOM_RIGHT].fg_green += green;
						block[GLYPH_QUAD_TOP_LEFT_BOTTOM_RIGHT].fg_blue += blue;
					} else if (block_x >= 2 && block_y < 4) {
						block[GLYPH_QUAD_TOP_LEFT].bg_red += red;
						block[GLYPH_QUAD_TOP_LEFT].bg_green += green;
						block[GLYPH_QUAD_TOP_LEFT].bg_blue += blue;

						block[GLYPH_QUAD_TOP_RIGHT].fg_red += red;
						block[GLYPH_QUAD_TOP_RIGHT].fg_green += green;
						block[GLYPH_QUAD_TOP_RIGHT].fg_blue += blue;

						block[GLYPH_QUAD_BOTTOM_LEFT].bg_red += red;
						block[GLYPH_QUAD_BOTTOM_LEFT].bg_green += green;
						block[GLYPH_QUAD_BOTTOM_LEFT].bg_blue += blue;

						block[GLYPH_QUAD_BOTTOM_RIGHT].bg_red += red;
						block[GLYPH_QUAD_BOTTOM_RIGHT].bg_green += green;
						block[GLYPH_QUAD_BOTTOM_RIGHT].bg_blue += blue;

						block[GLYPH_QUAD_TOP_LEFT_BOTTOM_RIGHT].bg_red += red;
						block[GLYPH_QUAD_TOP_LEFT_BOTTOM_RIGHT].bg_green += green;
						block[GLYPH_QUAD_TOP_LEFT_BOTTOM_RIGHT].bg_blue += blue;
					} else if (block_x < 2 && block_y >= 4) {
						block[GLYPH_QUAD_TOP_LEFT].bg_red += red;
						block[GLYPH_QUAD_TOP_LEFT].bg_green += green;
						block[GLYPH_QUAD_TOP_LEFT].bg_blue += blue;

						block[GLYPH_QUAD_TOP_RIGHT].bg_red += red;
						block[GLYPH_QUAD_TOP_RIGHT].bg_green += green;
						block[GLYPH_QUAD_TOP_RIGHT].bg_blue += blue;

						block[GLYPH_QUAD_BOTTOM_LEFT].fg_red += red;
						block[GLYPH_QUAD_BOTTOM_LEFT].fg_green += green;
						block[GLYPH_QUAD_BOTTOM_LEFT].fg_blue += blue;

						block[GLYPH_QUAD_BOTTOM_RIGHT].bg_red += red;
						block[GLYPH_QUAD_BOTTOM_RIGHT].bg_green += green;
						block[GLYPH_QUAD_BOTTOM_RIGHT].bg_blue += blue;

						block[GLYPH_QUAD_TOP_LEFT_BOTTOM_RIGHT].bg_red += red;
						block[GLYPH_QUAD_TOP_LEFT_BOTTOM_RIGHT].bg_green += green;
						block[GLYPH_QUAD_TOP_LEFT_BOTTOM_RIGHT].bg_blue += blue;
					} else if (block_x >= 2 && block_y >= 4) {
						block[GLYPH_QUAD_TOP_LEFT].bg_red += red;
						block[GLYPH_QUAD_TOP_LEFT].bg_green += green;
						block[GLYPH_QUAD_TOP_LEFT].bg_blue += blue;

						block[GLYPH_QUAD_TOP_RIGHT].bg_red += red;
						block[GLYPH_QUAD_TOP_RIGHT].bg_green += green;
						block[GLYPH_QUAD_TOP_RIGHT].bg_blue += blue;

						block[GLYPH_QUAD_BOTTOM_LEFT].bg_red += red;
						block[GLYPH_QUAD_BOTTOM_LEFT].bg_green += green;
						block[GLYPH_QUAD_BOTTOM_LEFT].bg_blue += blue;

						block[GLYPH_QUAD_BOTTOM_RIGHT].fg_red += red;
						block[GLYPH_QUAD_BOTTOM_RIGHT].fg_green += green;
						block[GLYPH_QUAD_BOTTOM_RIGHT].fg_blue += blue;

						block[GLYPH_QUAD_TOP_LEFT_BOTTOM_RIGHT].fg_red += red;
						block[GLYPH_QUAD_TOP_LEFT_BOTTOM_RIGHT].fg_green += green;
						block[GLYPH_QUAD_TOP_LEFT_BOTTOM_RIGHT].fg_blue += blue;
					}
				}

				block_y++;
			}

			iter = DestroyPixelIterator(iter);

			block[GLYPH_BLOCK].fg_red /= 32;
			block[GLYPH_BLOCK].fg_green /= 32;
			block[GLYPH_BLOCK].fg_blue /= 32;
			block[GLYPH_BLOCK].bg_red /= 32;
			block[GLYPH_BLOCK].bg_green /= 32;
			block[GLYPH_BLOCK].bg_blue /= 32;

			block[GLYPH_LEFT_1_4].fg_red /= 8;
			block[GLYPH_LEFT_1_4].fg_green /= 8;
			block[GLYPH_LEFT_1_4].fg_blue /= 8;
			block[GLYPH_LEFT_1_4].bg_red /= 24;
			block[GLYPH_LEFT_1_4].bg_green /= 24;
			block[GLYPH_LEFT_1_4].bg_blue /= 24;

			block[GLYPH_LEFT_2_4].fg_red /= 16;
			block[GLYPH_LEFT_2_4].fg_green /= 16;
			block[GLYPH_LEFT_2_4].fg_blue /= 16;
			block[GLYPH_LEFT_2_4].bg_red /= 16;
			block[GLYPH_LEFT_2_4].bg_green /= 16;
			block[GLYPH_LEFT_2_4].bg_blue /= 16;

			block[GLYPH_LEFT_3_4].fg_red /= 24;
			block[GLYPH_LEFT_3_4].fg_green /= 24;
			block[GLYPH_LEFT_3_4].fg_blue /= 24;
			block[GLYPH_LEFT_3_4].bg_red /= 8;
			block[GLYPH_LEFT_3_4].bg_green /= 8;
			block[GLYPH_LEFT_3_4].bg_blue /= 8;

			block[GLYPH_BOTTOM_1_8].fg_red /= 4;
			block[GLYPH_BOTTOM_1_8].fg_green /= 4;
			block[GLYPH_BOTTOM_1_8].fg_blue /= 4;
			block[GLYPH_BOTTOM_1_8].bg_red /= 28;
			block[GLYPH_BOTTOM_1_8].bg_green /= 28;
			block[GLYPH_BOTTOM_1_8].bg_blue /= 28;

			block[GLYPH_BOTTOM_2_8].fg_red /= 8;
			block[GLYPH_BOTTOM_2_8].fg_green /= 8;
			block[GLYPH_BOTTOM_2_8].fg_blue /= 8;
			block[GLYPH_BOTTOM_2_8].bg_red /= 24;
			block[GLYPH_BOTTOM_2_8].bg_green /= 24;
			block[GLYPH_BOTTOM_2_8].bg_blue /= 24;

			block[GLYPH_BOTTOM_3_8].fg_red /= 12;
			block[GLYPH_BOTTOM_3_8].fg_green /= 12;
			block[GLYPH_BOTTOM_3_8].fg_blue /= 12;
			block[GLYPH_BOTTOM_3_8].bg_red /= 20;
			block[GLYPH_BOTTOM_3_8].bg_green /= 20;
			block[GLYPH_BOTTOM_3_8].bg_blue /= 20;

			block[GLYPH_BOTTOM_4_8].fg_red /= 16;
			block[GLYPH_BOTTOM_4_8].fg_green /= 16;
			block[GLYPH_BOTTOM_4_8].fg_blue /= 16;
			block[GLYPH_BOTTOM_4_8].bg_red /= 16;
			block[GLYPH_BOTTOM_4_8].bg_green /= 16;
			block[GLYPH_BOTTOM_4_8].bg_blue /= 16;

			block[GLYPH_BOTTOM_5_8].fg_red /= 20;
			block[GLYPH_BOTTOM_5_8].fg_green /= 20;
			block[GLYPH_BOTTOM_5_8].fg_blue /= 20;
			block[GLYPH_BOTTOM_5_8].bg_red /= 12;
			block[GLYPH_BOTTOM_5_8].bg_green /= 12;
			block[GLYPH_BOTTOM_5_8].bg_blue /= 12;

			block[GLYPH_BOTTOM_6_8].fg_red /= 24;
			block[GLYPH_BOTTOM_6_8].fg_green /= 24;
			block[GLYPH_BOTTOM_6_8].fg_blue /= 24;
			block[GLYPH_BOTTOM_6_8].bg_red /= 8;
			block[GLYPH_BOTTOM_6_8].bg_green /= 8;
			block[GLYPH_BOTTOM_6_8].bg_blue /= 8;

			block[GLYPH_BOTTOM_7_8].fg_red /= 28;
			block[GLYPH_BOTTOM_7_8].fg_green /= 28;
			block[GLYPH_BOTTOM_7_8].fg_blue /= 28;
			block[GLYPH_BOTTOM_7_8].bg_red /= 4;
			block[GLYPH_BOTTOM_7_8].bg_green /= 4;
			block[GLYPH_BOTTOM_7_8].bg_blue /= 4;

			block[GLYPH_QUAD_TOP_LEFT].fg_red /= 8;
			block[GLYPH_QUAD_TOP_LEFT].fg_green /= 8;
			block[GLYPH_QUAD_TOP_LEFT].fg_blue /= 8;
			block[GLYPH_QUAD_TOP_LEFT].bg_red /= 24;
			block[GLYPH_QUAD_TOP_LEFT].bg_green /= 24;
			block[GLYPH_QUAD_TOP_LEFT].bg_blue /= 24;

			block[GLYPH_QUAD_TOP_RIGHT].fg_red /= 8;
			block[GLYPH_QUAD_TOP_RIGHT].fg_green /= 8;
			block[GLYPH_QUAD_TOP_RIGHT].fg_blue /= 8;
			block[GLYPH_QUAD_TOP_RIGHT].bg_red /= 24;
			block[GLYPH_QUAD_TOP_RIGHT].bg_green /= 24;
			block[GLYPH_QUAD_TOP_RIGHT].bg_blue /= 24;

			block[GLYPH_QUAD_BOTTOM_LEFT].fg_red /= 8;
			block[GLYPH_QUAD_BOTTOM_LEFT].fg_green /= 8;
			block[GLYPH_QUAD_BOTTOM_LEFT].fg_blue /= 8;
			block[GLYPH_QUAD_BOTTOM_LEFT].bg_red /= 24;
			block[GLYPH_QUAD_BOTTOM_LEFT].bg_green /= 24;
			block[GLYPH_QUAD_BOTTOM_LEFT].bg_blue /= 24;

			block[GLYPH_QUAD_BOTTOM_RIGHT].fg_red /= 8;
			block[GLYPH_QUAD_BOTTOM_RIGHT].fg_green /= 8;
			block[GLYPH_QUAD_BOTTOM_RIGHT].fg_blue /= 8;
			block[GLYPH_QUAD_BOTTOM_RIGHT].bg_red /= 24;
			block[GLYPH_QUAD_BOTTOM_RIGHT].bg_green /= 24;
			block[GLYPH_QUAD_BOTTOM_RIGHT].bg_blue /= 24;

			block[GLYPH_QUAD_TOP_LEFT_BOTTOM_RIGHT].fg_red /= 16;
			block[GLYPH_QUAD_TOP_LEFT_BOTTOM_RIGHT].fg_green /= 16;
			block[GLYPH_QUAD_TOP_LEFT_BOTTOM_RIGHT].fg_blue /= 16;
			block[GLYPH_QUAD_TOP_LEFT_BOTTOM_RIGHT].bg_red /= 16;
			block[GLYPH_QUAD_TOP_LEFT_BOTTOM_RIGHT].bg_green /= 16;
			block[GLYPH_QUAD_TOP_LEFT_BOTTOM_RIGHT].bg_blue /= 16;

			GlyphIndex index = 0;
			const char *glyph = getBestGlyph(&block, &index);

			printf("\x1b[38;2;%d;%d;%d;48;2;%d;%d;%dm%s\x1b[0m",
			       block[index].fg_red,
			       block[index].fg_green,
			       block[index].fg_blue,
			       block[index].bg_red,
			       block[index].bg_green,
			       block[index].bg_blue,
			       glyph);
		}

		printf("\n");
	}

	wand = DestroyMagickWand(wand);
	MagickWandTerminus();

	return EXIT_SUCCESS;
}
