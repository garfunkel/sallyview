#if __has_include(<MagickWand/MagickWand.h>)
	#include <MagickWand/MagickWand.h>
#else
	#include <wand/MagickWand.h>
#endif
#include <getopt.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

/*
 * Glyph IDs/indexes (for arrays).
 */
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

/*
 * Glyph characters.
 */
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

/*
 * Global definitions.
 */
#define NUM_DIFFERENT_GLYPHS 16
#define GlyphIndex int

/*
 * Return the difference between two numbers.
 */
#define diff(a, b) a >= b ? a - b : b - a

/*
 * Convenient for adding to the foreground colour channels of a block.
 */
#define add_colour_to_fg(x) \
	x.fg_red += red;        \
	x.fg_green += green;    \
	x.fg_blue += blue;

/*
 * Convenient for adding to the background colour channels of a block.
 */
#define add_colour_to_bg(x) \
	x.bg_red += red;        \
	x.bg_green += green;    \
	x.bg_blue += blue;

/*
 * Convenient for averaging foreground/background colour channels.
 */
#define divide_fg_bg_colour(x, y, z) \
	x.fg_red /= y;                   \
	x.fg_green /= y;                 \
	x.fg_blue /= y;                  \
	x.bg_red /= z;                   \
	x.bg_green /= z;                 \
	x.bg_blue /= z;

/*
 * Represents a glyph, with foreground and background colours.
 */
typedef struct {
	unsigned int fg_red;
	unsigned int fg_green;
	unsigned int fg_blue;
	unsigned int bg_red;
	unsigned int bg_green;
	unsigned int bg_blue;
} Glyph;

/*
 * Log a fatal ImageMagick error and exit.
 */
void log_error_exit(const MagickWand *wand) {
	ExceptionType severity;
	char *description = MagickGetException(wand, &severity);

	fprintf(stderr, "Error: %s:%s:%lu - %s\n", GetMagickModule(), description);

	description = MagickRelinquishMemory(description);
	MagickCoreTerminus();

	exit(EXIT_FAILURE);
}

/*
 * Return the glyph that best represents the block of pixels.
 * Also updates the best_glyph pointer with the best glyph's index.
 */
const char *get_best_glyph(const Glyph (*const block)[], GlyphIndex *best_glyph) {
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

	switch (*best_glyph) {
		case GLYPH_LEFT_1_4:
			return GLYPH_LEFT_1_4_STR;
		case GLYPH_LEFT_2_4:
			return GLYPH_LEFT_2_4_STR;
		case GLYPH_LEFT_3_4:
			return GLYPH_LEFT_3_4_STR;
		case GLYPH_BOTTOM_1_8:
			return GLYPH_BOTTOM_1_8_STR;
		case GLYPH_BOTTOM_2_8:
			return GLYPH_BOTTOM_2_8_STR;
		case GLYPH_BOTTOM_3_8:
			return GLYPH_BOTTOM_3_8_STR;
		case GLYPH_BOTTOM_4_8:
			return GLYPH_BOTTOM_4_8_STR;
		case GLYPH_BOTTOM_5_8:
			return GLYPH_BOTTOM_5_8_STR;
		case GLYPH_BOTTOM_6_8:
			return GLYPH_BOTTOM_6_8_STR;
		case GLYPH_BOTTOM_7_8:
			return GLYPH_BOTTOM_7_8_STR;
		case GLYPH_QUAD_TOP_LEFT:
			return GLYPH_QUAD_TOP_LEFT_STR;
		case GLYPH_QUAD_TOP_RIGHT:
			return GLYPH_QUAD_TOP_RIGHT_STR;
		case GLYPH_QUAD_BOTTOM_LEFT:
			return GLYPH_QUAD_BOTTOM_LEFT_STR;
		case GLYPH_QUAD_BOTTOM_RIGHT:
			return GLYPH_QUAD_BOTTOM_RIGHT_STR;
		case GLYPH_QUAD_TOP_LEFT_BOTTOM_RIGHT:
			return GLYPH_QUAD_TOP_LEFT_BOTTOM_RIGHT_STR;
	}

	return GLYPH_BLOCK_STR;
}

void usage(const char *prog) {
	printf("Usage: %s FILE...\n"
	       "View image FILE in a terminal/console.\n"
	       "Example: %s dog.jpg house.png\n",
	       prog,
	       prog);
}

int main(int argc, char *argv[]) {
	static struct option long_opts[] = {{"help", no_argument, NULL, 'h'}, {NULL, 0, NULL, 0}};
	int ch = 0;

	while ((ch = getopt_long(argc, argv, ":h", long_opts, NULL)) != -1) {
		switch (ch) {
			case 'h':
				usage(argv[0]);

				return EXIT_SUCCESS;
		}
	}

	if (argc <= 1) {
		fprintf(stderr, "No input file specified.\n");

		return EXIT_FAILURE;
	}

	MagickWandGenesis();

	MagickWand *wand = NewMagickWand();
	MagickBooleanType status = MagickReadImage(wand, argv[1]);

	if (status == MagickFalse) {
		log_error_exit(wand);
	}

	size_t width = MagickGetImageWidth(wand);
	size_t height = MagickGetImageHeight(wand);
	struct winsize window = {0};
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &window);
	size_t max_width = window.ws_col * 4;
	size_t max_height = window.ws_row * 8;

	if (width > max_width || height > max_height) {
		float ratio = (float)height / width;
		width = max_width;
		height = width * ratio;
		status = MagickResizeImage(wand, width, height, CubicFilter);

		if (status == MagickFalse) {
			log_error_exit(wand);
		}
	}

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
			Glyph block[NUM_DIFFERENT_GLYPHS] = {0};

			while ((row = PixelGetNextIteratorRow(iter, &num_wands)) != NULL) {
				for (size_t block_x = 0; block_x < num_wands; block_x++) {
					// Convert from ratio to decimal.
					unsigned int red = 255 * PixelGetRed(row[block_x]);
					unsigned int green = 255 * PixelGetGreen(row[block_x]);
					unsigned int blue = 255 * PixelGetBlue(row[block_x]);

					/*
					 * Calculate if each pixel is in the foreground/background for each glyph.
					 */
					add_colour_to_fg(block[GLYPH_BLOCK]);
					add_colour_to_bg(block[GLYPH_BLOCK]);

					if (block_x == 0) {
						add_colour_to_fg(block[GLYPH_LEFT_1_4]);
						add_colour_to_fg(block[GLYPH_LEFT_2_4]);
						add_colour_to_fg(block[GLYPH_LEFT_3_4]);
					} else if (block_x == 1) {
						add_colour_to_bg(block[GLYPH_LEFT_1_4]);
						add_colour_to_fg(block[GLYPH_LEFT_2_4]);
						add_colour_to_fg(block[GLYPH_LEFT_3_4]);
					} else if (block_x == 2) {
						add_colour_to_bg(block[GLYPH_LEFT_1_4]);
						add_colour_to_bg(block[GLYPH_LEFT_2_4]);
						add_colour_to_fg(block[GLYPH_LEFT_3_4]);
					} else if (block_x == 3) {
						add_colour_to_bg(block[GLYPH_LEFT_1_4]);
						add_colour_to_bg(block[GLYPH_LEFT_2_4]);
						add_colour_to_bg(block[GLYPH_LEFT_3_4]);
					}

					if (block_y == 0) {
						add_colour_to_bg(block[GLYPH_BOTTOM_1_8]);
						add_colour_to_bg(block[GLYPH_BOTTOM_2_8]);
						add_colour_to_bg(block[GLYPH_BOTTOM_3_8]);
						add_colour_to_bg(block[GLYPH_BOTTOM_4_8]);
						add_colour_to_bg(block[GLYPH_BOTTOM_5_8]);
						add_colour_to_bg(block[GLYPH_BOTTOM_6_8]);
						add_colour_to_bg(block[GLYPH_BOTTOM_7_8]);
					} else if (block_y == 1) {
						add_colour_to_bg(block[GLYPH_BOTTOM_1_8]);
						add_colour_to_bg(block[GLYPH_BOTTOM_2_8]);
						add_colour_to_bg(block[GLYPH_BOTTOM_3_8]);
						add_colour_to_bg(block[GLYPH_BOTTOM_4_8]);
						add_colour_to_bg(block[GLYPH_BOTTOM_5_8]);
						add_colour_to_bg(block[GLYPH_BOTTOM_6_8]);
						add_colour_to_fg(block[GLYPH_BOTTOM_7_8]);
					} else if (block_y == 2) {
						add_colour_to_bg(block[GLYPH_BOTTOM_1_8]);
						add_colour_to_bg(block[GLYPH_BOTTOM_2_8]);
						add_colour_to_bg(block[GLYPH_BOTTOM_3_8]);
						add_colour_to_bg(block[GLYPH_BOTTOM_4_8]);
						add_colour_to_bg(block[GLYPH_BOTTOM_5_8]);
						add_colour_to_fg(block[GLYPH_BOTTOM_6_8]);
						add_colour_to_fg(block[GLYPH_BOTTOM_7_8]);
					} else if (block_y == 3) {
						add_colour_to_bg(block[GLYPH_BOTTOM_1_8]);
						add_colour_to_bg(block[GLYPH_BOTTOM_2_8]);
						add_colour_to_bg(block[GLYPH_BOTTOM_3_8]);
						add_colour_to_bg(block[GLYPH_BOTTOM_4_8]);
						add_colour_to_fg(block[GLYPH_BOTTOM_5_8]);
						add_colour_to_fg(block[GLYPH_BOTTOM_6_8]);
						add_colour_to_fg(block[GLYPH_BOTTOM_7_8]);
					} else if (block_y == 4) {
						add_colour_to_bg(block[GLYPH_BOTTOM_1_8]);
						add_colour_to_bg(block[GLYPH_BOTTOM_2_8]);
						add_colour_to_bg(block[GLYPH_BOTTOM_3_8]);
						add_colour_to_fg(block[GLYPH_BOTTOM_4_8]);
						add_colour_to_fg(block[GLYPH_BOTTOM_5_8]);
						add_colour_to_fg(block[GLYPH_BOTTOM_6_8]);
						add_colour_to_fg(block[GLYPH_BOTTOM_7_8]);
					} else if (block_y == 5) {
						add_colour_to_bg(block[GLYPH_BOTTOM_1_8]);
						add_colour_to_bg(block[GLYPH_BOTTOM_2_8]);
						add_colour_to_fg(block[GLYPH_BOTTOM_3_8]);
						add_colour_to_fg(block[GLYPH_BOTTOM_4_8]);
						add_colour_to_fg(block[GLYPH_BOTTOM_5_8]);
						add_colour_to_fg(block[GLYPH_BOTTOM_6_8]);
						add_colour_to_fg(block[GLYPH_BOTTOM_7_8]);
					} else if (block_y == 6) {
						add_colour_to_bg(block[GLYPH_BOTTOM_1_8]);
						add_colour_to_fg(block[GLYPH_BOTTOM_2_8]);
						add_colour_to_fg(block[GLYPH_BOTTOM_3_8]);
						add_colour_to_fg(block[GLYPH_BOTTOM_4_8]);
						add_colour_to_fg(block[GLYPH_BOTTOM_5_8]);
						add_colour_to_fg(block[GLYPH_BOTTOM_6_8]);
						add_colour_to_fg(block[GLYPH_BOTTOM_7_8]);
					} else if (block_y == 7) {
						add_colour_to_fg(block[GLYPH_BOTTOM_1_8]);
						add_colour_to_fg(block[GLYPH_BOTTOM_2_8]);
						add_colour_to_fg(block[GLYPH_BOTTOM_3_8]);
						add_colour_to_fg(block[GLYPH_BOTTOM_4_8]);
						add_colour_to_fg(block[GLYPH_BOTTOM_5_8]);
						add_colour_to_fg(block[GLYPH_BOTTOM_6_8]);
						add_colour_to_fg(block[GLYPH_BOTTOM_7_8]);
					}

					if (block_x < 2 && block_y < 4) {
						add_colour_to_fg(block[GLYPH_QUAD_TOP_LEFT]);
						add_colour_to_bg(block[GLYPH_QUAD_TOP_RIGHT]);
						add_colour_to_bg(block[GLYPH_QUAD_BOTTOM_LEFT]);
						add_colour_to_bg(block[GLYPH_QUAD_BOTTOM_RIGHT]);
						add_colour_to_fg(block[GLYPH_QUAD_TOP_LEFT_BOTTOM_RIGHT]);
					} else if (block_x >= 2 && block_y < 4) {
						add_colour_to_bg(block[GLYPH_QUAD_TOP_LEFT]);
						add_colour_to_fg(block[GLYPH_QUAD_TOP_RIGHT]);
						add_colour_to_bg(block[GLYPH_QUAD_BOTTOM_LEFT]);
						add_colour_to_bg(block[GLYPH_QUAD_BOTTOM_RIGHT]);
						add_colour_to_bg(block[GLYPH_QUAD_TOP_LEFT_BOTTOM_RIGHT]);
					} else if (block_x < 2 && block_y >= 4) {
						add_colour_to_bg(block[GLYPH_QUAD_TOP_LEFT]);
						add_colour_to_bg(block[GLYPH_QUAD_TOP_RIGHT]);
						add_colour_to_fg(block[GLYPH_QUAD_BOTTOM_LEFT]);
						add_colour_to_bg(block[GLYPH_QUAD_BOTTOM_RIGHT]);
						add_colour_to_bg(block[GLYPH_QUAD_TOP_LEFT_BOTTOM_RIGHT]);
					} else if (block_x >= 2 && block_y >= 4) {
						add_colour_to_bg(block[GLYPH_QUAD_TOP_LEFT]);
						add_colour_to_bg(block[GLYPH_QUAD_TOP_RIGHT]);
						add_colour_to_bg(block[GLYPH_QUAD_BOTTOM_LEFT]);
						add_colour_to_fg(block[GLYPH_QUAD_BOTTOM_RIGHT]);
						add_colour_to_fg(block[GLYPH_QUAD_TOP_LEFT_BOTTOM_RIGHT]);
					}
				}

				block_y++;
			}

			iter = DestroyPixelIterator(iter);

			/*
			 * Calculate average for foreground/background colours for each glyph.
			 */
			divide_fg_bg_colour(block[GLYPH_BLOCK], 32, 32);
			divide_fg_bg_colour(block[GLYPH_LEFT_1_4], 8, 24);
			divide_fg_bg_colour(block[GLYPH_LEFT_2_4], 16, 16);
			divide_fg_bg_colour(block[GLYPH_LEFT_3_4], 24, 8);
			divide_fg_bg_colour(block[GLYPH_BOTTOM_1_8], 4, 28);
			divide_fg_bg_colour(block[GLYPH_BOTTOM_2_8], 8, 24);
			divide_fg_bg_colour(block[GLYPH_BOTTOM_3_8], 12, 20);
			divide_fg_bg_colour(block[GLYPH_BOTTOM_4_8], 16, 16);
			divide_fg_bg_colour(block[GLYPH_BOTTOM_5_8], 20, 12);
			divide_fg_bg_colour(block[GLYPH_BOTTOM_6_8], 24, 8);
			divide_fg_bg_colour(block[GLYPH_BOTTOM_7_8], 28, 4);
			divide_fg_bg_colour(block[GLYPH_QUAD_TOP_LEFT], 8, 24);
			divide_fg_bg_colour(block[GLYPH_QUAD_TOP_RIGHT], 8, 24);
			divide_fg_bg_colour(block[GLYPH_QUAD_BOTTOM_LEFT], 8, 24);
			divide_fg_bg_colour(block[GLYPH_QUAD_BOTTOM_RIGHT], 8, 24);
			divide_fg_bg_colour(block[GLYPH_QUAD_TOP_LEFT_BOTTOM_RIGHT], 16, 16);

			// Find the best glyph which represents this block of pixels.
			GlyphIndex index = 0;
			const char *glyph = get_best_glyph(&block, &index);

			// Print the glyph with the appropriate foreground/background colour.
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
