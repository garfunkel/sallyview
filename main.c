#if __has_include(<MagickWand/MagickWand.h>)
	#include <MagickWand/MagickWand.h>
#else
	#include <wand/MagickWand.h>
#endif
#include <getopt.h>
#include <stdio.h>
#include <string.h>
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
 * String values for various DACs.
 */
#define DAC_VGA "vga"
#define DAC_WIN_XP "win-xp"
#define DAC_POWERSHELL "powershell"
#define DAC_VS_CODE "vs-code"
#define DAC_WIN_10 "win-10"
#define DAC_TERMINAL_APP "terminal.app"
#define DAC_PUTTY "putty"
#define DAC_MIRC "mirc"
#define DAC_XTERM "xterm"
#define DAC_LINUX "linux"
#define DAC_ECLIPSE "eclipse"

/*
 * String values for various colour depths.
 */
#define COLOUR_DEPTH_3 "3"
#define COLOUR_DEPTH_4 "4"
#define COLOUR_DEPTH_8 "8"
#define COLOUR_DEPTH_24 "24"
#define DEFAULT_COLOUR_DEPTH COLOUR_DEPTH_24

/*
 * ANSI colour codes.
 */
#define ANSI_FG_BLACK 30
#define ANSI_FG_RED 31
#define ANSI_FG_GREEN 32
#define ANSI_FG_YELLOW 33
#define ANSI_FG_BLUE 34
#define ANSI_FG_MAGENTA 35
#define ANSI_FG_CYAN 36
#define ANSI_FG_WHITE 37

#define ANSI_BG_BLACK 40
#define ANSI_BG_RED 41
#define ANSI_BG_GREEN 42
#define ANSI_BG_YELLOW 43
#define ANSI_BG_BLUE 44
#define ANSI_BG_MAGENTA 45
#define ANSI_BG_CYAN 46
#define ANSI_BG_WHITE 47

#define ANSI_FG_BRIGHT_BLACK 90
#define ANSI_FG_BRIGHT_RED 91
#define ANSI_FG_BRIGHT_GREEN 92
#define ANSI_FG_BRIGHT_YELLOW 93
#define ANSI_FG_BRIGHT_BLUE 94
#define ANSI_FG_BRIGHT_MAGENTA 95
#define ANSI_FG_BRIGHT_CYAN 96
#define ANSI_FG_BRIGHT_WHITE 97

#define ANSI_BG_BRIGHT_BLACK 100
#define ANSI_BG_BRIGHT_RED 101
#define ANSI_BG_BRIGHT_GREEN 102
#define ANSI_BG_BRIGHT_YELLOW 103
#define ANSI_BG_BRIGHT_BLUE 104
#define ANSI_BG_BRIGHT_MAGENTA 105
#define ANSI_BG_BRIGHT_CYAN 106
#define ANSI_BG_BRIGHT_WHITE 107

/*
 * DAC colour mappings.
 */
#define DAC_VGA_BLACK \
	{ .red = COLOUR_0, .green = COLOUR_0, .blue = COLOUR_0, .ansi_fg = ANSI_FG_BLACK, .ansi_bg = ANSI_BG_BLACK }
#define DAC_VGA_RED \
	{ .red = 170, .green = COLOUR_0, .blue = COLOUR_0, .ansi_fg = ANSI_FG_RED, .ansi_bg = ANSI_BG_RED }
#define DAC_VGA_GREEN \
	{ .red = COLOUR_0, .green = 170, .blue = COLOUR_0, .ansi_fg = ANSI_FG_GREEN, .ansi_bg = ANSI_BG_GREEN }
#define DAC_VGA_YELLOW \
	{ .red = 170, .green = 85, .blue = COLOUR_0, .ansi_fg = ANSI_FG_YELLOW, .ansi_bg = ANSI_BG_YELLOW }
#define DAC_VGA_BLUE \
	{ .red = COLOUR_0, .green = COLOUR_0, .blue = 170, .ansi_fg = ANSI_FG_BLUE, .ansi_bg = ANSI_BG_BLUE }
#define DAC_VGA_MAGENTA \
	{ .red = 170, .green = COLOUR_0, .blue = 170, .ansi_fg = ANSI_FG_MAGENTA, .ansi_bg = ANSI_BG_MAGENTA }
#define DAC_VGA_CYAN \
	{ .red = COLOUR_0, .green = 170, .blue = 170, .ansi_fg = ANSI_FG_CYAN, .ansi_bg = ANSI_BG_CYAN }
#define DAC_VGA_WHITE \
	{ .red = 170, .green = 170, .blue = 170, .ansi_fg = ANSI_FG_WHITE, .ansi_bg = ANSI_BG_WHITE }
#define DAC_VGA_BRIGHT_BLACK \
	{ .red = 85, .green = 85, .blue = 85, .ansi_fg = ANSI_FG_BRIGHT_BLACK, .ansi_bg = ANSI_BG_BRIGHT_BLACK }
#define DAC_VGA_BRIGHT_RED \
	{ .red = COLOUR_255, .green = 85, .blue = 85, .ansi_fg = ANSI_FG_BRIGHT_RED, .ansi_bg = ANSI_BG_BRIGHT_RED }
#define DAC_VGA_BRIGHT_GREEN \
	{ .red = 85, .green = COLOUR_255, .blue = 85, .ansi_fg = ANSI_FG_BRIGHT_GREEN, .ansi_bg = ANSI_BG_BRIGHT_GREEN }
#define DAC_VGA_BRIGHT_YELLOW                                                                 \
	{                                                                                         \
		.red = COLOUR_255, .green = COLOUR_255, .blue = 85, .ansi_fg = ANSI_FG_BRIGHT_YELLOW, \
		.ansi_bg = ANSI_BG_BRIGHT_YELLOW                                                      \
	}
#define DAC_VGA_BRIGHT_BLUE \
	{ .red = 85, .green = 85, .blue = COLOUR_255, .ansi_fg = ANSI_FG_BRIGHT_BLUE, .ansi_bg = ANSI_BG_BRIGHT_BLUE }
#define DAC_VGA_BRIGHT_MAGENTA                                                                 \
	{                                                                                          \
		.red = COLOUR_255, .green = 85, .blue = COLOUR_255, .ansi_fg = ANSI_FG_BRIGHT_MAGENTA, \
		.ansi_bg = ANSI_BG_BRIGHT_MAGENTA                                                      \
	}
#define DAC_VGA_BRIGHT_CYAN                                                                 \
	{                                                                                       \
		.red = 85, .green = COLOUR_255, .blue = COLOUR_255, .ansi_fg = ANSI_FG_BRIGHT_CYAN, \
		.ansi_bg = ANSI_BG_BRIGHT_CYAN                                                      \
	}
#define DAC_VGA_BRIGHT_WHITE                                                                         \
	{                                                                                                \
		.red = COLOUR_255, .green = COLOUR_255, .blue = COLOUR_255, .ansi_fg = ANSI_FG_BRIGHT_WHITE, \
		.ansi_bg = ANSI_BG_BRIGHT_WHITE                                                              \
	}

#define DAC_WIN_XP_BLACK \
	{ .red = COLOUR_0, .green = COLOUR_0, .blue = COLOUR_0, .ansi_fg = ANSI_FG_BLACK, .ansi_bg = ANSI_BG_BLACK }
#define DAC_WIN_XP_RED \
	{ .red = COLOUR_128, .green = COLOUR_0, .blue = COLOUR_0, .ansi_fg = ANSI_FG_RED, .ansi_bg = ANSI_BG_RED }
#define DAC_WIN_XP_GREEN \
	{ .red = COLOUR_0, .green = COLOUR_128, .blue = COLOUR_0, .ansi_fg = ANSI_FG_GREEN, .ansi_bg = ANSI_BG_GREEN }
#define DAC_WIN_XP_YELLOW \
	{ .red = COLOUR_128, .green = COLOUR_128, .blue = COLOUR_0, .ansi_fg = ANSI_FG_YELLOW, .ansi_bg = ANSI_BG_YELLOW }
#define DAC_WIN_XP_BLUE \
	{ .red = COLOUR_0, .green = COLOUR_0, .blue = COLOUR_128, .ansi_fg = ANSI_FG_BLUE, .ansi_bg = ANSI_BG_BLUE }
#define DAC_WIN_XP_MAGENTA \
	{ .red = COLOUR_128, .green = COLOUR_0, .blue = COLOUR_128, .ansi_fg = ANSI_FG_MAGENTA, .ansi_bg = ANSI_BG_MAGENTA }
#define DAC_WIN_XP_CYAN \
	{ .red = COLOUR_0, .green = COLOUR_128, .blue = COLOUR_128, .ansi_fg = ANSI_FG_CYAN, .ansi_bg = ANSI_BG_CYAN }
#define DAC_WIN_XP_WHITE \
	{ .red = COLOUR_192, .green = COLOUR_192, .blue = COLOUR_192, .ansi_fg = ANSI_FG_WHITE, .ansi_bg = ANSI_BG_WHITE }
#define DAC_WIN_XP_BRIGHT_BLACK                                                                      \
	{                                                                                                \
		.red = COLOUR_128, .green = COLOUR_128, .blue = COLOUR_128, .ansi_fg = ANSI_FG_BRIGHT_BLACK, \
		.ansi_bg = ANSI_BG_BRIGHT_BLACK                                                              \
	}
#define DAC_WIN_XP_BRIGHT_RED                                                                  \
	{                                                                                          \
		.red = COLOUR_255, .green = COLOUR_0, .blue = COLOUR_0, .ansi_fg = ANSI_FG_BRIGHT_RED, \
		.ansi_bg = ANSI_BG_BRIGHT_RED                                                          \
	}
#define DAC_WIN_XP_BRIGHT_GREEN                                                                  \
	{                                                                                            \
		.red = COLOUR_0, .green = COLOUR_255, .blue = COLOUR_0, .ansi_fg = ANSI_FG_BRIGHT_GREEN, \
		.ansi_bg = ANSI_BG_BRIGHT_GREEN                                                          \
	}
#define DAC_WIN_XP_BRIGHT_YELLOW                                                                    \
	{                                                                                               \
		.red = COLOUR_255, .green = COLOUR_255, .blue = COLOUR_0, .ansi_fg = ANSI_FG_BRIGHT_YELLOW, \
		.ansi_bg = ANSI_BG_BRIGHT_YELLOW                                                            \
	}
#define DAC_WIN_XP_BRIGHT_BLUE                                                                  \
	{                                                                                           \
		.red = COLOUR_0, .green = COLOUR_0, .blue = COLOUR_255, .ansi_fg = ANSI_FG_BRIGHT_BLUE, \
		.ansi_bg = ANSI_BG_BRIGHT_BLUE                                                          \
	}
#define DAC_WIN_XP_BRIGHT_MAGENTA                                                                    \
	{                                                                                                \
		.red = COLOUR_255, .green = COLOUR_0, .blue = COLOUR_255, .ansi_fg = ANSI_FG_BRIGHT_MAGENTA, \
		.ansi_bg = ANSI_BG_BRIGHT_MAGENTA                                                            \
	}
#define DAC_WIN_XP_BRIGHT_CYAN                                                                    \
	{                                                                                             \
		.red = COLOUR_0, .green = COLOUR_255, .blue = COLOUR_255, .ansi_fg = ANSI_FG_BRIGHT_CYAN, \
		.ansi_bg = ANSI_BG_BRIGHT_CYAN                                                            \
	}
#define DAC_WIN_XP_BRIGHT_WHITE                                                                      \
	{                                                                                                \
		.red = COLOUR_255, .green = COLOUR_255, .blue = COLOUR_255, .ansi_fg = ANSI_FG_BRIGHT_WHITE, \
		.ansi_bg = ANSI_BG_BRIGHT_WHITE                                                              \
	}

#define DAC_POWERSHELL_BLACK \
	{ .red = COLOUR_0, .green = COLOUR_0, .blue = COLOUR_0, .ansi_fg = ANSI_FG_BLACK, .ansi_bg = ANSI_BG_BLACK }
#define DAC_POWERSHELL_RED \
	{ .red = COLOUR_128, .green = COLOUR_0, .blue = COLOUR_0, .ansi_fg = ANSI_FG_RED, .ansi_bg = ANSI_BG_RED }
#define DAC_POWERSHELL_GREEN \
	{ .red = COLOUR_0, .green = COLOUR_128, .blue = COLOUR_0, .ansi_fg = ANSI_FG_GREEN, .ansi_bg = ANSI_BG_GREEN }
#define DAC_POWERSHELL_YELLOW \
	{ .red = 238, .green = 237, .blue = 240, .ansi_fg = ANSI_FG_YELLOW, .ansi_bg = ANSI_BG_YELLOW }
#define DAC_POWERSHELL_BLUE \
	{ .red = COLOUR_0, .green = COLOUR_0, .blue = COLOUR_128, .ansi_fg = ANSI_FG_BLUE, .ansi_bg = ANSI_BG_BLUE }
#define DAC_POWERSHELL_MAGENTA \
	{ .red = 1, .green = 36, .blue = 86, .ansi_fg = ANSI_FG_MAGENTA, .ansi_bg = ANSI_BG_MAGENTA }
#define DAC_POWERSHELL_CYAN \
	{ .red = COLOUR_0, .green = COLOUR_128, .blue = COLOUR_128, .ansi_fg = ANSI_FG_CYAN, .ansi_bg = ANSI_BG_CYAN }
#define DAC_POWERSHELL_WHITE \
	{ .red = COLOUR_192, .green = COLOUR_192, .blue = COLOUR_192, .ansi_fg = ANSI_FG_WHITE, .ansi_bg = ANSI_BG_WHITE }
#define DAC_POWERSHELL_BRIGHT_BLACK                                                                  \
	{                                                                                                \
		.red = COLOUR_128, .green = COLOUR_128, .blue = COLOUR_128, .ansi_fg = ANSI_FG_BRIGHT_BLACK, \
		.ansi_bg = ANSI_BG_BRIGHT_BLACK                                                              \
	}
#define DAC_POWERSHELL_BRIGHT_RED                                                              \
	{                                                                                          \
		.red = COLOUR_255, .green = COLOUR_0, .blue = COLOUR_0, .ansi_fg = ANSI_FG_BRIGHT_RED, \
		.ansi_bg = ANSI_BG_BRIGHT_RED                                                          \
	}
#define DAC_POWERSHELL_BRIGHT_GREEN                                                              \
	{                                                                                            \
		.red = COLOUR_0, .green = COLOUR_255, .blue = COLOUR_0, .ansi_fg = ANSI_FG_BRIGHT_GREEN, \
		.ansi_bg = ANSI_BG_BRIGHT_GREEN                                                          \
	}
#define DAC_POWERSHELL_BRIGHT_YELLOW                                                                \
	{                                                                                               \
		.red = COLOUR_255, .green = COLOUR_255, .blue = COLOUR_0, .ansi_fg = ANSI_FG_BRIGHT_YELLOW, \
		.ansi_bg = ANSI_BG_BRIGHT_YELLOW                                                            \
	}
#define DAC_POWERSHELL_BRIGHT_BLUE                                                              \
	{                                                                                           \
		.red = COLOUR_0, .green = COLOUR_0, .blue = COLOUR_255, .ansi_fg = ANSI_FG_BRIGHT_BLUE, \
		.ansi_bg = ANSI_BG_BRIGHT_BLUE                                                          \
	}
#define DAC_POWERSHELL_BRIGHT_MAGENTA                                                                \
	{                                                                                                \
		.red = COLOUR_255, .green = COLOUR_0, .blue = COLOUR_255, .ansi_fg = ANSI_FG_BRIGHT_MAGENTA, \
		.ansi_bg = ANSI_BG_BRIGHT_MAGENTA                                                            \
	}
#define DAC_POWERSHELL_BRIGHT_CYAN                                                                \
	{                                                                                             \
		.red = COLOUR_0, .green = COLOUR_255, .blue = COLOUR_255, .ansi_fg = ANSI_FG_BRIGHT_CYAN, \
		.ansi_bg = ANSI_BG_BRIGHT_CYAN                                                            \
	}
#define DAC_POWERSHELL_BRIGHT_WHITE                                                                  \
	{                                                                                                \
		.red = COLOUR_255, .green = COLOUR_255, .blue = COLOUR_255, .ansi_fg = ANSI_FG_BRIGHT_WHITE, \
		.ansi_bg = ANSI_BG_BRIGHT_WHITE                                                              \
	}

#define DAC_VS_CODE_BLACK \
	{ .red = COLOUR_0, .green = COLOUR_0, .blue = COLOUR_0, .ansi_fg = ANSI_FG_BLACK, .ansi_bg = ANSI_BG_BLACK }
#define DAC_VS_CODE_RED \
	{ .red = 205, .green = 49, .blue = 49, .ansi_fg = ANSI_FG_RED, .ansi_bg = ANSI_BG_RED }
#define DAC_VS_CODE_GREEN \
	{ .red = 13, .green = 188, .blue = 121, .ansi_fg = ANSI_FG_GREEN, .ansi_bg = ANSI_BG_GREEN }
#define DAC_VS_CODE_YELLOW \
	{ .red = 229, .green = 229, .blue = 16, .ansi_fg = ANSI_FG_YELLOW, .ansi_bg = ANSI_BG_YELLOW }
#define DAC_VS_CODE_BLUE \
	{ .red = 36, .green = 114, .blue = 200, .ansi_fg = ANSI_FG_BLUE, .ansi_bg = ANSI_BG_BLUE }
#define DAC_VS_CODE_MAGENTA \
	{ .red = 188, .green = 63, .blue = 188, .ansi_fg = ANSI_FG_MAGENTA, .ansi_bg = ANSI_BG_MAGENTA }
#define DAC_VS_CODE_CYAN \
	{ .red = 17, .green = 168, .blue = 205, .ansi_fg = ANSI_FG_CYAN, .ansi_bg = ANSI_BG_CYAN }
#define DAC_VS_CODE_WHITE \
	{ .red = 229, .green = 229, .blue = 229, .ansi_fg = ANSI_FG_WHITE, .ansi_bg = ANSI_BG_WHITE }
#define DAC_VS_CODE_BRIGHT_BLACK \
	{ .red = 102, .green = 102, .blue = 102, .ansi_fg = ANSI_FG_BRIGHT_BLACK, .ansi_bg = ANSI_BG_BRIGHT_BLACK }
#define DAC_VS_CODE_BRIGHT_RED \
	{ .red = 241, .green = 76, .blue = 76, .ansi_fg = ANSI_FG_BRIGHT_RED, .ansi_bg = ANSI_BG_BRIGHT_RED }
#define DAC_VS_CODE_BRIGHT_GREEN \
	{ .red = 35, .green = 209, .blue = 139, .ansi_fg = ANSI_FG_BRIGHT_GREEN, .ansi_bg = ANSI_BG_BRIGHT_GREEN }
#define DAC_VS_CODE_BRIGHT_YELLOW \
	{ .red = 245, .green = 245, .blue = 67, .ansi_fg = ANSI_FG_BRIGHT_YELLOW, .ansi_bg = ANSI_BG_BRIGHT_YELLOW }
#define DAC_VS_CODE_BRIGHT_BLUE \
	{ .red = 59, .green = 142, .blue = 234, .ansi_fg = ANSI_FG_BRIGHT_BLUE, .ansi_bg = ANSI_BG_BRIGHT_BLUE }
#define DAC_VS_CODE_BRIGHT_MAGENTA \
	{ .red = 214, .green = 112, .blue = 214, .ansi_fg = ANSI_FG_BRIGHT_MAGENTA, .ansi_bg = ANSI_BG_BRIGHT_MAGENTA }
#define DAC_VS_CODE_BRIGHT_CYAN \
	{ .red = 41, .green = 184, .blue = 219, .ansi_fg = ANSI_FG_BRIGHT_CYAN, .ansi_bg = ANSI_BG_BRIGHT_CYAN }
#define DAC_VS_CODE_BRIGHT_WHITE \
	{ .red = 229, .green = 229, .blue = 229, .ansi_fg = ANSI_FG_BRIGHT_WHITE, .ansi_bg = ANSI_BG_BRIGHT_WHITE }

#define DAC_WIN_10_BLACK \
	{ .red = 12, .green = 12, .blue = 12, .ansi_fg = ANSI_FG_BLACK, .ansi_bg = ANSI_BG_BLACK }
#define DAC_WIN_10_RED \
	{ .red = 197, .green = 15, .blue = 31, .ansi_fg = ANSI_FG_RED, .ansi_bg = ANSI_BG_RED }
#define DAC_WIN_10_GREEN \
	{ .red = 19, .green = 161, .blue = 14, .ansi_fg = ANSI_FG_GREEN, .ansi_bg = ANSI_BG_GREEN }
#define DAC_WIN_10_YELLOW \
	{ .red = 193, .green = 156, .blue = COLOUR_0, .ansi_fg = ANSI_FG_YELLOW, .ansi_bg = ANSI_BG_YELLOW }
#define DAC_WIN_10_BLUE \
	{ .red = COLOUR_0, .green = 55, .blue = 218, .ansi_fg = ANSI_FG_BLUE, .ansi_bg = ANSI_BG_BLUE }
#define DAC_WIN_10_MAGENTA \
	{ .red = 136, .green = 23, .blue = 152, .ansi_fg = ANSI_FG_MAGENTA, .ansi_bg = ANSI_BG_MAGENTA }
#define DAC_WIN_10_CYAN \
	{ .red = 58, .green = 150, .blue = 221, .ansi_fg = ANSI_FG_CYAN, .ansi_bg = ANSI_BG_CYAN }
#define DAC_WIN_10_WHITE \
	{ .red = 204, .green = 204, .blue = 204, .ansi_fg = ANSI_FG_WHITE, .ansi_bg = ANSI_BG_WHITE }
#define DAC_WIN_10_BRIGHT_BLACK \
	{ .red = 118, .green = 118, .blue = 118, .ansi_fg = ANSI_FG_BRIGHT_BLACK, .ansi_bg = ANSI_BG_BRIGHT_BLACK }
#define DAC_WIN_10_BRIGHT_RED \
	{ .red = 231, .green = 72, .blue = 86, .ansi_fg = ANSI_FG_BRIGHT_RED, .ansi_bg = ANSI_BG_BRIGHT_RED }
#define DAC_WIN_10_BRIGHT_GREEN \
	{ .red = 22, .green = 198, .blue = 12, .ansi_fg = ANSI_FG_BRIGHT_GREEN, .ansi_bg = ANSI_BG_BRIGHT_GREEN }
#define DAC_WIN_10_BRIGHT_YELLOW \
	{ .red = 249, .green = 241, .blue = 165, .ansi_fg = ANSI_FG_BRIGHT_YELLOW, .ansi_bg = ANSI_BG_BRIGHT_YELLOW }
#define DAC_WIN_10_BRIGHT_BLUE \
	{ .red = 59, .green = 120, .blue = COLOUR_255, .ansi_fg = ANSI_FG_BRIGHT_BLUE, .ansi_bg = ANSI_BG_BRIGHT_BLUE }
#define DAC_WIN_10_BRIGHT_MAGENTA \
	{ .red = 180, .green = COLOUR_0, .blue = 158, .ansi_fg = ANSI_FG_BRIGHT_MAGENTA, .ansi_bg = ANSI_BG_BRIGHT_MAGENTA }
#define DAC_WIN_10_BRIGHT_CYAN \
	{ .red = 97, .green = 214, .blue = 214, .ansi_fg = ANSI_FG_BRIGHT_CYAN, .ansi_bg = ANSI_BG_BRIGHT_CYAN }
#define DAC_WIN_10_BRIGHT_WHITE \
	{ .red = 242, .green = 242, .blue = 242, .ansi_fg = ANSI_FG_BRIGHT_WHITE, .ansi_bg = ANSI_BG_BRIGHT_WHITE }

#define DAC_TERMINAL_APP_BLACK \
	{ .red = COLOUR_0, .green = COLOUR_0, .blue = COLOUR_0, .ansi_fg = ANSI_FG_BLACK, .ansi_bg = ANSI_BG_BLACK }
#define DAC_TERMINAL_APP_RED \
	{ .red = 194, .green = 54, .blue = 33, .ansi_fg = ANSI_FG_RED, .ansi_bg = ANSI_BG_RED }
#define DAC_TERMINAL_APP_GREEN \
	{ .red = 37, .green = 188, .blue = 36, .ansi_fg = ANSI_FG_GREEN, .ansi_bg = ANSI_BG_GREEN }
#define DAC_TERMINAL_APP_YELLOW \
	{ .red = 173, .green = 173, .blue = 39, .ansi_fg = ANSI_FG_YELLOW, .ansi_bg = ANSI_BG_YELLOW }
#define DAC_TERMINAL_APP_BLUE \
	{ .red = 73, .green = 46, .blue = 225, .ansi_fg = ANSI_FG_BLUE, .ansi_bg = ANSI_BG_BLUE }
#define DAC_TERMINAL_APP_MAGENTA \
	{ .red = 211, .green = 56, .blue = 211, .ansi_fg = ANSI_FG_MAGENTA, .ansi_bg = ANSI_BG_MAGENTA }
#define DAC_TERMINAL_APP_CYAN \
	{ .red = 51, .green = 187, .blue = 200, .ansi_fg = ANSI_FG_CYAN, .ansi_bg = ANSI_BG_CYAN }
#define DAC_TERMINAL_APP_WHITE \
	{ .red = 203, .green = 204, .blue = 205, .ansi_fg = ANSI_FG_WHITE, .ansi_bg = ANSI_BG_WHITE }
#define DAC_TERMINAL_APP_BRIGHT_BLACK \
	{ .red = 129, .green = 131, .blue = 131, .ansi_fg = ANSI_FG_BRIGHT_BLACK, .ansi_bg = ANSI_BG_BRIGHT_BLACK }
#define DAC_TERMINAL_APP_BRIGHT_RED \
	{ .red = 252, .green = 57, .blue = 31, .ansi_fg = ANSI_FG_BRIGHT_RED, .ansi_bg = ANSI_BG_BRIGHT_RED }
#define DAC_TERMINAL_APP_BRIGHT_GREEN \
	{ .red = 49, .green = 231, .blue = 34, .ansi_fg = ANSI_FG_BRIGHT_GREEN, .ansi_bg = ANSI_BG_BRIGHT_GREEN }
#define DAC_TERMINAL_APP_BRIGHT_YELLOW \
	{ .red = 234, .green = 236, .blue = 35, .ansi_fg = ANSI_FG_BRIGHT_YELLOW, .ansi_bg = ANSI_BG_BRIGHT_YELLOW }
#define DAC_TERMINAL_APP_BRIGHT_BLUE \
	{ .red = 88, .green = 51, .blue = COLOUR_255, .ansi_fg = ANSI_FG_BRIGHT_BLUE, .ansi_bg = ANSI_BG_BRIGHT_BLUE }
#define DAC_TERMINAL_APP_BRIGHT_MAGENTA \
	{ .red = 249, .green = 53, .blue = 248, .ansi_fg = ANSI_FG_BRIGHT_MAGENTA, .ansi_bg = ANSI_BG_BRIGHT_MAGENTA }
#define DAC_TERMINAL_APP_BRIGHT_CYAN \
	{ .red = 20, .green = 240, .blue = 240, .ansi_fg = ANSI_FG_BRIGHT_CYAN, .ansi_bg = ANSI_BG_BRIGHT_CYAN }
#define DAC_TERMINAL_APP_BRIGHT_WHITE \
	{ .red = 233, .green = 235, .blue = 235, .ansi_fg = ANSI_FG_BRIGHT_WHITE, .ansi_bg = ANSI_BG_BRIGHT_WHITE }

#define DAC_PUTTY_BLACK \
	{ .red = COLOUR_0, .green = COLOUR_0, .blue = COLOUR_0, .ansi_fg = ANSI_FG_BLACK, .ansi_bg = ANSI_BG_BLACK }
#define DAC_PUTTY_RED \
	{ .red = 187, .green = COLOUR_0, .blue = COLOUR_0, .ansi_fg = ANSI_FG_RED, .ansi_bg = ANSI_BG_RED }
#define DAC_PUTTY_GREEN \
	{ .red = COLOUR_0, .green = 187, .blue = COLOUR_0, .ansi_fg = ANSI_FG_GREEN, .ansi_bg = ANSI_BG_GREEN }
#define DAC_PUTTY_YELLOW \
	{ .red = 187, .green = 187, .blue = COLOUR_0, .ansi_fg = ANSI_FG_YELLOW, .ansi_bg = ANSI_BG_YELLOW }
#define DAC_PUTTY_BLUE \
	{ .red = COLOUR_0, .green = COLOUR_0, .blue = 187, .ansi_fg = ANSI_FG_BLUE, .ansi_bg = ANSI_BG_BLUE }
#define DAC_PUTTY_MAGENTA \
	{ .red = 187, .green = COLOUR_0, .blue = 187, .ansi_fg = ANSI_FG_MAGENTA, .ansi_bg = ANSI_BG_MAGENTA }
#define DAC_PUTTY_CYAN \
	{ .red = COLOUR_0, .green = 187, .blue = 187, .ansi_fg = ANSI_FG_CYAN, .ansi_bg = ANSI_BG_CYAN }
#define DAC_PUTTY_WHITE \
	{ .red = 187, .green = 187, .blue = 187, .ansi_fg = ANSI_FG_WHITE, .ansi_bg = ANSI_BG_WHITE }
#define DAC_PUTTY_BRIGHT_BLACK \
	{ .red = 85, .green = 85, .blue = 85, .ansi_fg = ANSI_FG_BRIGHT_BLACK, .ansi_bg = ANSI_BG_BRIGHT_BLACK }
#define DAC_PUTTY_BRIGHT_RED \
	{ .red = COLOUR_255, .green = 85, .blue = 85, .ansi_fg = ANSI_FG_BRIGHT_RED, .ansi_bg = ANSI_BG_BRIGHT_RED }
#define DAC_PUTTY_BRIGHT_GREEN \
	{ .red = 85, .green = COLOUR_255, .blue = 85, .ansi_fg = ANSI_FG_BRIGHT_GREEN, .ansi_bg = ANSI_BG_BRIGHT_GREEN }
#define DAC_PUTTY_BRIGHT_YELLOW                                                               \
	{                                                                                         \
		.red = COLOUR_255, .green = COLOUR_255, .blue = 85, .ansi_fg = ANSI_FG_BRIGHT_YELLOW, \
		.ansi_bg = ANSI_BG_BRIGHT_YELLOW                                                      \
	}
#define DAC_PUTTY_BRIGHT_BLUE \
	{ .red = 85, .green = 85, .blue = COLOUR_255, .ansi_fg = ANSI_FG_BRIGHT_BLUE, .ansi_bg = ANSI_BG_BRIGHT_BLUE }
#define DAC_PUTTY_BRIGHT_MAGENTA                                                               \
	{                                                                                          \
		.red = COLOUR_255, .green = 85, .blue = COLOUR_255, .ansi_fg = ANSI_FG_BRIGHT_MAGENTA, \
		.ansi_bg = ANSI_BG_BRIGHT_MAGENTA                                                      \
	}
#define DAC_PUTTY_BRIGHT_CYAN                                                               \
	{                                                                                       \
		.red = 85, .green = COLOUR_255, .blue = COLOUR_255, .ansi_fg = ANSI_FG_BRIGHT_CYAN, \
		.ansi_bg = ANSI_BG_BRIGHT_CYAN                                                      \
	}
#define DAC_PUTTY_BRIGHT_WHITE                                                                       \
	{                                                                                                \
		.red = COLOUR_255, .green = COLOUR_255, .blue = COLOUR_255, .ansi_fg = ANSI_FG_BRIGHT_WHITE, \
		.ansi_bg = ANSI_BG_BRIGHT_WHITE                                                              \
	}

#define DAC_MIRC_BLACK \
	{ .red = COLOUR_0, .green = COLOUR_0, .blue = COLOUR_0, .ansi_fg = ANSI_FG_BLACK, .ansi_bg = ANSI_BG_BLACK }
#define DAC_MIRC_RED \
	{ .red = 127, .green = COLOUR_0, .blue = COLOUR_0, .ansi_fg = ANSI_FG_RED, .ansi_bg = ANSI_BG_RED }
#define DAC_MIRC_GREEN \
	{ .red = COLOUR_0, .green = 147, .blue = COLOUR_0, .ansi_fg = ANSI_FG_GREEN, .ansi_bg = ANSI_BG_GREEN }
#define DAC_MIRC_YELLOW \
	{ .red = 252, .green = 127, .blue = COLOUR_0, .ansi_fg = ANSI_FG_YELLOW, .ansi_bg = ANSI_BG_YELLOW }
#define DAC_MIRC_BLUE \
	{ .red = COLOUR_0, .green = COLOUR_0, .blue = 127, .ansi_fg = ANSI_FG_BLUE, .ansi_bg = ANSI_BG_BLUE }
#define DAC_MIRC_MAGENTA \
	{ .red = 156, .green = COLOUR_0, .blue = 156, .ansi_fg = ANSI_FG_MAGENTA, .ansi_bg = ANSI_BG_MAGENTA }
#define DAC_MIRC_CYAN \
	{ .red = COLOUR_0, .green = 147, .blue = 147, .ansi_fg = ANSI_FG_CYAN, .ansi_bg = ANSI_BG_CYAN }
#define DAC_MIRC_WHITE \
	{ .red = 210, .green = 210, .blue = 210, .ansi_fg = ANSI_FG_WHITE, .ansi_bg = ANSI_BG_WHITE }
#define DAC_MIRC_BRIGHT_BLACK \
	{ .red = 127, .green = 127, .blue = 127, .ansi_fg = ANSI_FG_BRIGHT_BLACK, .ansi_bg = ANSI_BG_BRIGHT_BLACK }
#define DAC_MIRC_BRIGHT_RED                                                                    \
	{                                                                                          \
		.red = COLOUR_255, .green = COLOUR_0, .blue = COLOUR_0, .ansi_fg = ANSI_FG_BRIGHT_RED, \
		.ansi_bg = ANSI_BG_BRIGHT_RED                                                          \
	}
#define DAC_MIRC_BRIGHT_GREEN                                                             \
	{                                                                                     \
		.red = COLOUR_0, .green = 252, .blue = COLOUR_0, .ansi_fg = ANSI_FG_BRIGHT_GREEN, \
		.ansi_bg = ANSI_BG_BRIGHT_GREEN                                                   \
	}
#define DAC_MIRC_BRIGHT_YELLOW                                                                      \
	{                                                                                               \
		.red = COLOUR_255, .green = COLOUR_255, .blue = COLOUR_0, .ansi_fg = ANSI_FG_BRIGHT_YELLOW, \
		.ansi_bg = ANSI_BG_BRIGHT_YELLOW                                                            \
	}
#define DAC_MIRC_BRIGHT_BLUE \
	{ .red = COLOUR_0, .green = COLOUR_0, .blue = 252, .ansi_fg = ANSI_FG_BRIGHT_BLUE, .ansi_bg = ANSI_BG_BRIGHT_BLUE }
#define DAC_MIRC_BRIGHT_MAGENTA                                                                      \
	{                                                                                                \
		.red = COLOUR_255, .green = COLOUR_0, .blue = COLOUR_255, .ansi_fg = ANSI_FG_BRIGHT_MAGENTA, \
		.ansi_bg = ANSI_BG_BRIGHT_MAGENTA                                                            \
	}
#define DAC_MIRC_BRIGHT_CYAN                                                                      \
	{                                                                                             \
		.red = COLOUR_0, .green = COLOUR_255, .blue = COLOUR_255, .ansi_fg = ANSI_FG_BRIGHT_CYAN, \
		.ansi_bg = ANSI_BG_BRIGHT_CYAN                                                            \
	}
#define DAC_MIRC_BRIGHT_WHITE                                                                        \
	{                                                                                                \
		.red = COLOUR_255, .green = COLOUR_255, .blue = COLOUR_255, .ansi_fg = ANSI_FG_BRIGHT_WHITE, \
		.ansi_bg = ANSI_BG_BRIGHT_WHITE                                                              \
	}

#define DAC_XTERM_BLACK \
	{ .red = COLOUR_0, .green = COLOUR_0, .blue = COLOUR_0, .ansi_fg = ANSI_FG_BLACK, .ansi_bg = ANSI_BG_BLACK }
#define DAC_XTERM_RED \
	{ .red = 205, .green = COLOUR_0, .blue = COLOUR_0, .ansi_fg = ANSI_FG_RED, .ansi_bg = ANSI_BG_RED }
#define DAC_XTERM_GREEN \
	{ .red = COLOUR_0, .green = 205, .blue = COLOUR_0, .ansi_fg = ANSI_FG_GREEN, .ansi_bg = ANSI_BG_GREEN }
#define DAC_XTERM_YELLOW \
	{ .red = 205, .green = 205, .blue = COLOUR_0, .ansi_fg = ANSI_FG_YELLOW, .ansi_bg = ANSI_BG_YELLOW }
#define DAC_XTERM_BLUE \
	{ .red = COLOUR_0, .green = COLOUR_0, .blue = 238, .ansi_fg = ANSI_FG_BLUE, .ansi_bg = ANSI_BG_BLUE }
#define DAC_XTERM_MAGENTA \
	{ .red = 205, .green = COLOUR_0, .blue = 205, .ansi_fg = ANSI_FG_MAGENTA, .ansi_bg = ANSI_BG_MAGENTA }
#define DAC_XTERM_CYAN \
	{ .red = COLOUR_0, .green = 205, .blue = 205, .ansi_fg = ANSI_FG_CYAN, .ansi_bg = ANSI_BG_CYAN }
#define DAC_XTERM_WHITE \
	{ .red = 229, .green = 229, .blue = 229, .ansi_fg = ANSI_FG_WHITE, .ansi_bg = ANSI_BG_WHITE }
#define DAC_XTERM_BRIGHT_BLACK \
	{ .red = 127, .green = 127, .blue = 127, .ansi_fg = ANSI_FG_BRIGHT_BLACK, .ansi_bg = ANSI_BG_BRIGHT_BLACK }
#define DAC_XTERM_BRIGHT_RED                                                                   \
	{                                                                                          \
		.red = COLOUR_255, .green = COLOUR_0, .blue = COLOUR_0, .ansi_fg = ANSI_FG_BRIGHT_RED, \
		.ansi_bg = ANSI_BG_BRIGHT_RED                                                          \
	}
#define DAC_XTERM_BRIGHT_GREEN                                                                   \
	{                                                                                            \
		.red = COLOUR_0, .green = COLOUR_255, .blue = COLOUR_0, .ansi_fg = ANSI_FG_BRIGHT_GREEN, \
		.ansi_bg = ANSI_BG_BRIGHT_GREEN                                                          \
	}
#define DAC_XTERM_BRIGHT_YELLOW                                                                     \
	{                                                                                               \
		.red = COLOUR_255, .green = COLOUR_255, .blue = COLOUR_0, .ansi_fg = ANSI_FG_BRIGHT_YELLOW, \
		.ansi_bg = ANSI_BG_BRIGHT_YELLOW                                                            \
	}
#define DAC_XTERM_BRIGHT_BLUE \
	{ .red = 92, .green = 92, .blue = COLOUR_255, .ansi_fg = ANSI_FG_BRIGHT_BLUE, .ansi_bg = ANSI_BG_BRIGHT_BLUE }
#define DAC_XTERM_BRIGHT_MAGENTA                                                                     \
	{                                                                                                \
		.red = COLOUR_255, .green = COLOUR_0, .blue = COLOUR_255, .ansi_fg = ANSI_FG_BRIGHT_MAGENTA, \
		.ansi_bg = ANSI_BG_BRIGHT_MAGENTA                                                            \
	}
#define DAC_XTERM_BRIGHT_CYAN                                                                     \
	{                                                                                             \
		.red = COLOUR_0, .green = COLOUR_255, .blue = COLOUR_255, .ansi_fg = ANSI_FG_BRIGHT_CYAN, \
		.ansi_bg = ANSI_BG_BRIGHT_CYAN                                                            \
	}
#define DAC_XTERM_BRIGHT_WHITE                                                                       \
	{                                                                                                \
		.red = COLOUR_255, .green = COLOUR_255, .blue = COLOUR_255, .ansi_fg = ANSI_FG_BRIGHT_WHITE, \
		.ansi_bg = ANSI_BG_BRIGHT_WHITE                                                              \
	}

#define DAC_LINUX_BLACK \
	{ .red = 1, .green = 1, .blue = 2001, .ansi_fg = ANSI_FG_BLACK, .ansi_bg = ANSI_BG_BLACK }
#define DAC_LINUX_RED \
	{ .red = 222, .green = 56, .blue = 43, .ansi_fg = ANSI_FG_RED, .ansi_bg = ANSI_BG_RED }
#define DAC_LINUX_GREEN \
	{ .red = 57, .green = 181, .blue = 74, .ansi_fg = ANSI_FG_GREEN, .ansi_bg = ANSI_BG_GREEN }
#define DAC_LINUX_YELLOW \
	{ .red = COLOUR_255, .green = 199, .blue = 6, .ansi_fg = ANSI_FG_YELLOW, .ansi_bg = ANSI_BG_YELLOW }
#define DAC_LINUX_BLUE \
	{ .red = COLOUR_0, .green = 111, .blue = 184, .ansi_fg = ANSI_FG_BLUE, .ansi_bg = ANSI_BG_BLUE }
#define DAC_LINUX_MAGENTA \
	{ .red = 118, .green = 38, .blue = 113, .ansi_fg = ANSI_FG_MAGENTA, .ansi_bg = ANSI_BG_MAGENTA }
#define DAC_LINUX_CYAN \
	{ .red = 44, .green = 181, .blue = 233, .ansi_fg = ANSI_FG_CYAN, .ansi_bg = ANSI_BG_CYAN }
#define DAC_LINUX_WHITE \
	{ .red = 204, .green = 204, .blue = 204, .ansi_fg = ANSI_FG_WHITE, .ansi_bg = ANSI_BG_WHITE }
#define DAC_LINUX_BRIGHT_BLACK                                                                       \
	{                                                                                                \
		.red = COLOUR_128, .green = COLOUR_128, .blue = COLOUR_128, .ansi_fg = ANSI_FG_BRIGHT_BLACK, \
		.ansi_bg = ANSI_BG_BRIGHT_BLACK                                                              \
	}
#define DAC_LINUX_BRIGHT_RED                                                                   \
	{                                                                                          \
		.red = COLOUR_255, .green = COLOUR_0, .blue = COLOUR_0, .ansi_fg = ANSI_FG_BRIGHT_RED, \
		.ansi_bg = ANSI_BG_BRIGHT_RED                                                          \
	}
#define DAC_LINUX_BRIGHT_GREEN                                                                   \
	{                                                                                            \
		.red = COLOUR_0, .green = COLOUR_255, .blue = COLOUR_0, .ansi_fg = ANSI_FG_BRIGHT_GREEN, \
		.ansi_bg = ANSI_BG_BRIGHT_GREEN                                                          \
	}
#define DAC_LINUX_BRIGHT_YELLOW                                                                     \
	{                                                                                               \
		.red = COLOUR_255, .green = COLOUR_255, .blue = COLOUR_0, .ansi_fg = ANSI_FG_BRIGHT_YELLOW, \
		.ansi_bg = ANSI_BG_BRIGHT_YELLOW                                                            \
	}
#define DAC_LINUX_BRIGHT_BLUE                                                                   \
	{                                                                                           \
		.red = COLOUR_0, .green = COLOUR_0, .blue = COLOUR_255, .ansi_fg = ANSI_FG_BRIGHT_BLUE, \
		.ansi_bg = ANSI_BG_BRIGHT_BLUE                                                          \
	}
#define DAC_LINUX_BRIGHT_MAGENTA                                                                     \
	{                                                                                                \
		.red = COLOUR_255, .green = COLOUR_0, .blue = COLOUR_255, .ansi_fg = ANSI_FG_BRIGHT_MAGENTA, \
		.ansi_bg = ANSI_BG_BRIGHT_MAGENTA                                                            \
	}
#define DAC_LINUX_BRIGHT_CYAN                                                                     \
	{                                                                                             \
		.red = COLOUR_0, .green = COLOUR_255, .blue = COLOUR_255, .ansi_fg = ANSI_FG_BRIGHT_CYAN, \
		.ansi_bg = ANSI_BG_BRIGHT_CYAN                                                            \
	}
#define DAC_LINUX_BRIGHT_WHITE                                                                       \
	{                                                                                                \
		.red = COLOUR_255, .green = COLOUR_255, .blue = COLOUR_255, .ansi_fg = ANSI_FG_BRIGHT_WHITE, \
		.ansi_bg = ANSI_BG_BRIGHT_WHITE                                                              \
	}

#define DAC_ECLIPSE_BLACK \
	{ .red = COLOUR_0, .green = COLOUR_0, .blue = COLOUR_0, .ansi_fg = ANSI_FG_BLACK, .ansi_bg = ANSI_BG_BLACK }
#define DAC_ECLIPSE_RED \
	{ .red = 205, .green = COLOUR_0, .blue = COLOUR_0, .ansi_fg = ANSI_FG_RED, .ansi_bg = ANSI_BG_RED }
#define DAC_ECLIPSE_GREEN \
	{ .red = COLOUR_0, .green = 205, .blue = COLOUR_0, .ansi_fg = ANSI_FG_GREEN, .ansi_bg = ANSI_BG_GREEN }
#define DAC_ECLIPSE_YELLOW \
	{ .red = 205, .green = 205, .blue = COLOUR_0, .ansi_fg = ANSI_FG_YELLOW, .ansi_bg = ANSI_BG_YELLOW }
#define DAC_ECLIPSE_BLUE \
	{ .red = COLOUR_0, .green = COLOUR_0, .blue = 238, .ansi_fg = ANSI_FG_BLUE, .ansi_bg = ANSI_BG_BLUE }
#define DAC_ECLIPSE_MAGENTA \
	{ .red = 205, .green = COLOUR_0, .blue = 205, .ansi_fg = ANSI_FG_MAGENTA, .ansi_bg = ANSI_BG_MAGENTA }
#define DAC_ECLIPSE_CYAN \
	{ .red = 205, .green = COLOUR_0, .blue = 205, .ansi_fg = ANSI_FG_CYAN, .ansi_bg = ANSI_BG_CYAN }
#define DAC_ECLIPSE_WHITE \
	{ .red = 229, .green = 229, .blue = 229, .ansi_fg = ANSI_FG_WHITE, .ansi_bg = ANSI_BG_WHITE }
#define DAC_ECLIPSE_BRIGHT_BLACK                                                               \
	{                                                                                          \
		.red = COLOUR_0, .green = COLOUR_0, .blue = COLOUR_0, .ansi_fg = ANSI_FG_BRIGHT_BLACK, \
		.ansi_bg = ANSI_BG_BRIGHT_BLACK                                                        \
	}
#define DAC_ECLIPSE_BRIGHT_RED                                                                 \
	{                                                                                          \
		.red = COLOUR_255, .green = COLOUR_0, .blue = COLOUR_0, .ansi_fg = ANSI_FG_BRIGHT_RED, \
		.ansi_bg = ANSI_BG_BRIGHT_RED                                                          \
	}
#define DAC_ECLIPSE_BRIGHT_GREEN                                                                 \
	{                                                                                            \
		.red = COLOUR_0, .green = COLOUR_255, .blue = COLOUR_0, .ansi_fg = ANSI_FG_BRIGHT_GREEN, \
		.ansi_bg = ANSI_BG_BRIGHT_GREEN                                                          \
	}
#define DAC_ECLIPSE_BRIGHT_YELLOW                                                                   \
	{                                                                                               \
		.red = COLOUR_255, .green = COLOUR_255, .blue = COLOUR_0, .ansi_fg = ANSI_FG_BRIGHT_YELLOW, \
		.ansi_bg = ANSI_BG_BRIGHT_YELLOW                                                            \
	}
#define DAC_ECLIPSE_BRIGHT_BLUE \
	{ .red = 92, .green = 92, .blue = COLOUR_255, .ansi_fg = ANSI_FG_BRIGHT_BLUE, .ansi_bg = ANSI_BG_BRIGHT_BLUE }
#define DAC_ECLIPSE_BRIGHT_MAGENTA                                                                   \
	{                                                                                                \
		.red = COLOUR_255, .green = COLOUR_0, .blue = COLOUR_255, .ansi_fg = ANSI_FG_BRIGHT_MAGENTA, \
		.ansi_bg = ANSI_BG_BRIGHT_MAGENTA                                                            \
	}
#define DAC_ECLIPSE_BRIGHT_CYAN                                                                   \
	{                                                                                             \
		.red = COLOUR_0, .green = COLOUR_255, .blue = COLOUR_255, .ansi_fg = ANSI_FG_BRIGHT_CYAN, \
		.ansi_bg = ANSI_BG_BRIGHT_CYAN                                                            \
	}
#define DAC_ECLIPSE_BRIGHT_WHITE                                                                     \
	{                                                                                                \
		.red = COLOUR_255, .green = COLOUR_255, .blue = COLOUR_255, .ansi_fg = ANSI_FG_BRIGHT_WHITE, \
		.ansi_bg = ANSI_BG_BRIGHT_WHITE                                                              \
	}

/*
 * Global definitions.
 */
#define NUM_DIFFERENT_GLYPHS 16
#define GlyphIndex int
#define COLOUR_0 0
#define COLOUR_128 128
#define COLOUR_192 192
#define COLOUR_255 255
#define NUM_DAC_REGULAR_COLOURS 8
#define NUM_DAC_3_BIT_COLOURS 8
#define NUM_DAC_4_BIT_COLOURS 16
#define NUM_DAC_8_BIT_COLOURS 256
#define NUM_DACS 11

/*
 * 24-bit colour mappings for 3-4 bit ANSI.
 */
#ifdef __linux__
	#define DEFAULT_DAC DAC_LINUX
#elif __APPLE__
	#define DEFAULT_DAC DAC_TERMINAL_APP
#elif _WIN32
	#define DEFAULT_DAC DAC_WIN_10
#else
	#define DEFAULT_DAC DAC_VGA
#endif

/*
 * Return the difference between two numbers.
 */
#define diff(a, b) (a >= b ? a - b : b - a)

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

#define convert_to_greyscale(x)                    \
	int total = x.fg_red + x.fg_green + x.fg_blue; \
	x.fg_red = total / 3;                          \
	x.fg_green = total / 3;                        \
	x.fg_blue = total / 3;                         \
	total = x.bg_red + x.bg_green + x.bg_blue;     \
	x.bg_red = total / 3;                          \
	x.bg_green = total / 3;                        \
	x.bg_blue = total / 3;

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

typedef struct {
	unsigned int red;
	unsigned int green;
	unsigned int blue;
	int ansi_fg;
	int ansi_bg;
} DacColour;

typedef const struct {
	const char *dac;
	const DacColour colours[16];
} Dac;

static const Dac DACS[] = {{.dac = DAC_VGA,
                            .colours = {DAC_VGA_BLACK,
                                        DAC_VGA_RED,
                                        DAC_VGA_GREEN,
                                        DAC_VGA_YELLOW,
                                        DAC_VGA_BLUE,
                                        DAC_VGA_MAGENTA,
                                        DAC_VGA_CYAN,
                                        DAC_VGA_WHITE,
                                        DAC_VGA_BRIGHT_BLACK,
                                        DAC_VGA_BRIGHT_RED,
                                        DAC_VGA_BRIGHT_GREEN,
                                        DAC_VGA_BRIGHT_YELLOW,
                                        DAC_VGA_BRIGHT_BLUE,
                                        DAC_VGA_BRIGHT_MAGENTA,
                                        DAC_VGA_BRIGHT_CYAN,
                                        DAC_VGA_BRIGHT_WHITE}},
                           {.dac = DAC_WIN_XP,
                            .colours = {DAC_WIN_XP_BLACK,
                                        DAC_WIN_XP_RED,
                                        DAC_WIN_XP_GREEN,
                                        DAC_WIN_XP_YELLOW,
                                        DAC_WIN_XP_BLUE,
                                        DAC_WIN_XP_MAGENTA,
                                        DAC_WIN_XP_CYAN,
                                        DAC_WIN_XP_WHITE,
                                        DAC_WIN_XP_BRIGHT_BLACK,
                                        DAC_WIN_XP_BRIGHT_RED,
                                        DAC_WIN_XP_BRIGHT_GREEN,
                                        DAC_WIN_XP_BRIGHT_YELLOW,
                                        DAC_WIN_XP_BRIGHT_BLUE,
                                        DAC_WIN_XP_BRIGHT_MAGENTA,
                                        DAC_WIN_XP_BRIGHT_CYAN,
                                        DAC_WIN_XP_BRIGHT_WHITE}},
                           {.dac = DAC_POWERSHELL,
                            .colours = {DAC_POWERSHELL_BLACK,
                                        DAC_POWERSHELL_RED,
                                        DAC_POWERSHELL_GREEN,
                                        DAC_POWERSHELL_YELLOW,
                                        DAC_POWERSHELL_BLUE,
                                        DAC_POWERSHELL_MAGENTA,
                                        DAC_POWERSHELL_CYAN,
                                        DAC_POWERSHELL_WHITE,
                                        DAC_POWERSHELL_BRIGHT_BLACK,
                                        DAC_POWERSHELL_BRIGHT_RED,
                                        DAC_POWERSHELL_BRIGHT_GREEN,
                                        DAC_POWERSHELL_BRIGHT_YELLOW,
                                        DAC_POWERSHELL_BRIGHT_BLUE,
                                        DAC_POWERSHELL_BRIGHT_MAGENTA,
                                        DAC_POWERSHELL_BRIGHT_CYAN,
                                        DAC_POWERSHELL_BRIGHT_WHITE}},
                           {.dac = DAC_VS_CODE,
                            .colours = {DAC_VS_CODE_BLACK,
                                        DAC_VS_CODE_RED,
                                        DAC_VS_CODE_GREEN,
                                        DAC_VS_CODE_YELLOW,
                                        DAC_VS_CODE_BLUE,
                                        DAC_VS_CODE_MAGENTA,
                                        DAC_VS_CODE_CYAN,
                                        DAC_VS_CODE_WHITE,
                                        DAC_VS_CODE_BRIGHT_BLACK,
                                        DAC_VS_CODE_BRIGHT_RED,
                                        DAC_VS_CODE_BRIGHT_GREEN,
                                        DAC_VS_CODE_BRIGHT_YELLOW,
                                        DAC_VS_CODE_BRIGHT_BLUE,
                                        DAC_VS_CODE_BRIGHT_MAGENTA,
                                        DAC_VS_CODE_BRIGHT_CYAN,
                                        DAC_VS_CODE_BRIGHT_WHITE}},
                           {.dac = DAC_WIN_10,
                            .colours = {DAC_WIN_10_BLACK,
                                        DAC_WIN_10_RED,
                                        DAC_WIN_10_GREEN,
                                        DAC_WIN_10_YELLOW,
                                        DAC_WIN_10_BLUE,
                                        DAC_WIN_10_MAGENTA,
                                        DAC_WIN_10_CYAN,
                                        DAC_WIN_10_WHITE,
                                        DAC_WIN_10_BRIGHT_BLACK,
                                        DAC_WIN_10_BRIGHT_RED,
                                        DAC_WIN_10_BRIGHT_GREEN,
                                        DAC_WIN_10_BRIGHT_YELLOW,
                                        DAC_WIN_10_BRIGHT_BLUE,
                                        DAC_WIN_10_BRIGHT_MAGENTA,
                                        DAC_WIN_10_BRIGHT_CYAN,
                                        DAC_WIN_10_BRIGHT_WHITE}},
                           {.dac = DAC_TERMINAL_APP,
                            .colours = {DAC_TERMINAL_APP_BLACK,
                                        DAC_TERMINAL_APP_RED,
                                        DAC_TERMINAL_APP_GREEN,
                                        DAC_TERMINAL_APP_YELLOW,
                                        DAC_TERMINAL_APP_BLUE,
                                        DAC_TERMINAL_APP_MAGENTA,
                                        DAC_TERMINAL_APP_CYAN,
                                        DAC_TERMINAL_APP_WHITE,
                                        DAC_TERMINAL_APP_BRIGHT_BLACK,
                                        DAC_TERMINAL_APP_BRIGHT_RED,
                                        DAC_TERMINAL_APP_BRIGHT_GREEN,
                                        DAC_TERMINAL_APP_BRIGHT_YELLOW,
                                        DAC_TERMINAL_APP_BRIGHT_BLUE,
                                        DAC_TERMINAL_APP_BRIGHT_MAGENTA,
                                        DAC_TERMINAL_APP_BRIGHT_CYAN,
                                        DAC_TERMINAL_APP_BRIGHT_WHITE}},
                           {.dac = DAC_PUTTY,
                            .colours = {DAC_PUTTY_BLACK,
                                        DAC_PUTTY_RED,
                                        DAC_PUTTY_GREEN,
                                        DAC_PUTTY_YELLOW,
                                        DAC_PUTTY_BLUE,
                                        DAC_PUTTY_MAGENTA,
                                        DAC_PUTTY_CYAN,
                                        DAC_PUTTY_WHITE,
                                        DAC_PUTTY_BRIGHT_BLACK,
                                        DAC_PUTTY_BRIGHT_RED,
                                        DAC_PUTTY_BRIGHT_GREEN,
                                        DAC_PUTTY_BRIGHT_YELLOW,
                                        DAC_PUTTY_BRIGHT_BLUE,
                                        DAC_PUTTY_BRIGHT_MAGENTA,
                                        DAC_PUTTY_BRIGHT_CYAN,
                                        DAC_PUTTY_BRIGHT_WHITE}},
                           {.dac = DAC_MIRC,
                            .colours = {DAC_MIRC_BLACK,
                                        DAC_MIRC_RED,
                                        DAC_MIRC_GREEN,
                                        DAC_MIRC_YELLOW,
                                        DAC_MIRC_BLUE,
                                        DAC_MIRC_MAGENTA,
                                        DAC_MIRC_CYAN,
                                        DAC_MIRC_WHITE,
                                        DAC_MIRC_BRIGHT_BLACK,
                                        DAC_MIRC_BRIGHT_RED,
                                        DAC_MIRC_BRIGHT_GREEN,
                                        DAC_MIRC_BRIGHT_YELLOW,
                                        DAC_MIRC_BRIGHT_BLUE,
                                        DAC_MIRC_BRIGHT_MAGENTA,
                                        DAC_MIRC_BRIGHT_CYAN,
                                        DAC_MIRC_BRIGHT_WHITE}},
                           {.dac = DAC_XTERM,
                            .colours = {DAC_XTERM_BLACK,
                                        DAC_XTERM_RED,
                                        DAC_XTERM_GREEN,
                                        DAC_XTERM_YELLOW,
                                        DAC_XTERM_BLUE,
                                        DAC_XTERM_MAGENTA,
                                        DAC_XTERM_CYAN,
                                        DAC_XTERM_WHITE,
                                        DAC_XTERM_BRIGHT_BLACK,
                                        DAC_XTERM_BRIGHT_RED,
                                        DAC_XTERM_BRIGHT_GREEN,
                                        DAC_XTERM_BRIGHT_YELLOW,
                                        DAC_XTERM_BRIGHT_BLUE,
                                        DAC_XTERM_BRIGHT_MAGENTA,
                                        DAC_XTERM_BRIGHT_CYAN,
                                        DAC_XTERM_BRIGHT_WHITE}},
                           {.dac = DAC_LINUX,
                            .colours = {DAC_LINUX_BLACK,
                                        DAC_LINUX_RED,
                                        DAC_LINUX_GREEN,
                                        DAC_LINUX_YELLOW,
                                        DAC_LINUX_BLUE,
                                        DAC_LINUX_MAGENTA,
                                        DAC_LINUX_CYAN,
                                        DAC_LINUX_WHITE,
                                        DAC_LINUX_BRIGHT_BLACK,
                                        DAC_LINUX_BRIGHT_RED,
                                        DAC_LINUX_BRIGHT_GREEN,
                                        DAC_LINUX_BRIGHT_YELLOW,
                                        DAC_LINUX_BRIGHT_BLUE,
                                        DAC_LINUX_BRIGHT_MAGENTA,
                                        DAC_LINUX_BRIGHT_CYAN,
                                        DAC_LINUX_BRIGHT_WHITE}},
                           {.dac = DAC_ECLIPSE,
                            .colours = {DAC_ECLIPSE_BLACK,
                                        DAC_ECLIPSE_RED,
                                        DAC_ECLIPSE_GREEN,
                                        DAC_ECLIPSE_YELLOW,
                                        DAC_ECLIPSE_BLUE,
                                        DAC_ECLIPSE_MAGENTA,
                                        DAC_ECLIPSE_CYAN,
                                        DAC_ECLIPSE_WHITE,
                                        DAC_ECLIPSE_BRIGHT_BLACK,
                                        DAC_ECLIPSE_BRIGHT_RED,
                                        DAC_ECLIPSE_BRIGHT_GREEN,
                                        DAC_ECLIPSE_BRIGHT_YELLOW,
                                        DAC_ECLIPSE_BRIGHT_BLUE,
                                        DAC_ECLIPSE_BRIGHT_MAGENTA,
                                        DAC_ECLIPSE_BRIGHT_CYAN,
                                        DAC_ECLIPSE_BRIGHT_WHITE}}};

static const DacColour DAC_8_BIT_COLOURS[] = {
    {.red = 0, .green = 0, .blue = 0, .ansi_fg = 0, .ansi_bg = 0},
    {.red = 128, .green = 0, .blue = 0, .ansi_fg = 1, .ansi_bg = 1},
    {.red = 0, .green = 128, .blue = 0, .ansi_fg = 2, .ansi_bg = 2},
    {.red = 128, .green = 128, .blue = 0, .ansi_fg = 3, .ansi_bg = 3},
    {.red = 0, .green = 0, .blue = 128, .ansi_fg = 4, .ansi_bg = 4},
    {.red = 128, .green = 0, .blue = 128, .ansi_fg = 5, .ansi_bg = 5},
    {.red = 0, .green = 128, .blue = 128, .ansi_fg = 6, .ansi_bg = 6},
    {.red = 192, .green = 192, .blue = 192, .ansi_fg = 7, .ansi_bg = 7},
    {.red = 128, .green = 128, .blue = 128, .ansi_fg = 8, .ansi_bg = 8},
    {.red = 255, .green = 0, .blue = 0, .ansi_fg = 9, .ansi_bg = 9},
    {.red = 0, .green = 255, .blue = 0, .ansi_fg = 10, .ansi_bg = 10},
    {.red = 255, .green = 255, .blue = 0, .ansi_fg = 11, .ansi_bg = 11},
    {.red = 0, .green = 0, .blue = 255, .ansi_fg = 12, .ansi_bg = 12},
    {.red = 255, .green = 0, .blue = 255, .ansi_fg = 13, .ansi_bg = 13},
    {.red = 0, .green = 255, .blue = 255, .ansi_fg = 14, .ansi_bg = 14},
    {.red = 255, .green = 255, .blue = 255, .ansi_fg = 15, .ansi_bg = 15},
    {.red = 0, .green = 0, .blue = 0, .ansi_fg = 16, .ansi_bg = 16},
    {.red = 0, .green = 0, .blue = 95, .ansi_fg = 17, .ansi_bg = 17},
    {.red = 0, .green = 0, .blue = 135, .ansi_fg = 18, .ansi_bg = 18},
    {.red = 0, .green = 0, .blue = 175, .ansi_fg = 19, .ansi_bg = 19},
    {.red = 0, .green = 0, .blue = 215, .ansi_fg = 20, .ansi_bg = 20},
    {.red = 0, .green = 0, .blue = 255, .ansi_fg = 21, .ansi_bg = 21},
    {.red = 0, .green = 95, .blue = 0, .ansi_fg = 22, .ansi_bg = 22},
    {.red = 0, .green = 95, .blue = 95, .ansi_fg = 23, .ansi_bg = 23},
    {.red = 0, .green = 95, .blue = 135, .ansi_fg = 24, .ansi_bg = 24},
    {.red = 0, .green = 95, .blue = 175, .ansi_fg = 25, .ansi_bg = 25},
    {.red = 0, .green = 95, .blue = 215, .ansi_fg = 26, .ansi_bg = 26},
    {.red = 0, .green = 95, .blue = 255, .ansi_fg = 27, .ansi_bg = 27},
    {.red = 0, .green = 135, .blue = 0, .ansi_fg = 28, .ansi_bg = 28},
    {.red = 0, .green = 135, .blue = 95, .ansi_fg = 29, .ansi_bg = 29},
    {.red = 0, .green = 135, .blue = 135, .ansi_fg = 30, .ansi_bg = 30},
    {.red = 0, .green = 135, .blue = 175, .ansi_fg = 31, .ansi_bg = 31},
    {.red = 0, .green = 135, .blue = 215, .ansi_fg = 32, .ansi_bg = 32},
    {.red = 0, .green = 135, .blue = 255, .ansi_fg = 33, .ansi_bg = 33},
    {.red = 0, .green = 175, .blue = 0, .ansi_fg = 34, .ansi_bg = 34},
    {.red = 0, .green = 175, .blue = 95, .ansi_fg = 35, .ansi_bg = 35},
    {.red = 0, .green = 175, .blue = 135, .ansi_fg = 36, .ansi_bg = 36},
    {.red = 0, .green = 175, .blue = 175, .ansi_fg = 37, .ansi_bg = 37},
    {.red = 0, .green = 175, .blue = 215, .ansi_fg = 38, .ansi_bg = 38},
    {.red = 0, .green = 175, .blue = 255, .ansi_fg = 39, .ansi_bg = 39},
    {.red = 0, .green = 215, .blue = 0, .ansi_fg = 40, .ansi_bg = 40},
    {.red = 0, .green = 215, .blue = 95, .ansi_fg = 41, .ansi_bg = 41},
    {.red = 0, .green = 215, .blue = 135, .ansi_fg = 42, .ansi_bg = 42},
    {.red = 0, .green = 215, .blue = 175, .ansi_fg = 43, .ansi_bg = 43},
    {.red = 0, .green = 215, .blue = 215, .ansi_fg = 44, .ansi_bg = 44},
    {.red = 0, .green = 215, .blue = 255, .ansi_fg = 45, .ansi_bg = 45},
    {.red = 0, .green = 255, .blue = 0, .ansi_fg = 46, .ansi_bg = 46},
    {.red = 0, .green = 255, .blue = 95, .ansi_fg = 47, .ansi_bg = 47},
    {.red = 0, .green = 255, .blue = 135, .ansi_fg = 48, .ansi_bg = 48},
    {.red = 0, .green = 255, .blue = 175, .ansi_fg = 49, .ansi_bg = 49},
    {.red = 0, .green = 255, .blue = 215, .ansi_fg = 50, .ansi_bg = 50},
    {.red = 0, .green = 255, .blue = 255, .ansi_fg = 51, .ansi_bg = 51},
    {.red = 95, .green = 0, .blue = 0, .ansi_fg = 52, .ansi_bg = 52},
    {.red = 95, .green = 0, .blue = 95, .ansi_fg = 53, .ansi_bg = 53},
    {.red = 95, .green = 0, .blue = 135, .ansi_fg = 54, .ansi_bg = 54},
    {.red = 95, .green = 0, .blue = 175, .ansi_fg = 55, .ansi_bg = 55},
    {.red = 95, .green = 0, .blue = 215, .ansi_fg = 56, .ansi_bg = 56},
    {.red = 95, .green = 0, .blue = 255, .ansi_fg = 57, .ansi_bg = 57},
    {.red = 95, .green = 95, .blue = 0, .ansi_fg = 58, .ansi_bg = 58},
    {.red = 95, .green = 95, .blue = 95, .ansi_fg = 59, .ansi_bg = 59},
    {.red = 95, .green = 95, .blue = 135, .ansi_fg = 60, .ansi_bg = 60},
    {.red = 95, .green = 95, .blue = 175, .ansi_fg = 61, .ansi_bg = 61},
    {.red = 95, .green = 95, .blue = 215, .ansi_fg = 62, .ansi_bg = 62},
    {.red = 95, .green = 95, .blue = 255, .ansi_fg = 63, .ansi_bg = 63},
    {.red = 95, .green = 135, .blue = 0, .ansi_fg = 64, .ansi_bg = 64},
    {.red = 95, .green = 135, .blue = 95, .ansi_fg = 65, .ansi_bg = 65},
    {.red = 95, .green = 135, .blue = 135, .ansi_fg = 66, .ansi_bg = 66},
    {.red = 95, .green = 135, .blue = 175, .ansi_fg = 67, .ansi_bg = 67},
    {.red = 95, .green = 135, .blue = 215, .ansi_fg = 68, .ansi_bg = 68},
    {.red = 95, .green = 135, .blue = 255, .ansi_fg = 69, .ansi_bg = 69},
    {.red = 95, .green = 175, .blue = 0, .ansi_fg = 70, .ansi_bg = 70},
    {.red = 95, .green = 175, .blue = 95, .ansi_fg = 71, .ansi_bg = 71},
    {.red = 95, .green = 175, .blue = 135, .ansi_fg = 72, .ansi_bg = 72},
    {.red = 95, .green = 175, .blue = 175, .ansi_fg = 73, .ansi_bg = 73},
    {.red = 95, .green = 175, .blue = 215, .ansi_fg = 74, .ansi_bg = 74},
    {.red = 95, .green = 175, .blue = 255, .ansi_fg = 75, .ansi_bg = 75},
    {.red = 95, .green = 215, .blue = 0, .ansi_fg = 76, .ansi_bg = 76},
    {.red = 95, .green = 215, .blue = 95, .ansi_fg = 77, .ansi_bg = 77},
    {.red = 95, .green = 215, .blue = 135, .ansi_fg = 78, .ansi_bg = 78},
    {.red = 95, .green = 215, .blue = 175, .ansi_fg = 79, .ansi_bg = 79},
    {.red = 95, .green = 215, .blue = 215, .ansi_fg = 80, .ansi_bg = 80},
    {.red = 95, .green = 215, .blue = 255, .ansi_fg = 81, .ansi_bg = 81},
    {.red = 95, .green = 255, .blue = 0, .ansi_fg = 82, .ansi_bg = 82},
    {.red = 95, .green = 255, .blue = 95, .ansi_fg = 83, .ansi_bg = 83},
    {.red = 95, .green = 255, .blue = 135, .ansi_fg = 84, .ansi_bg = 84},
    {.red = 95, .green = 255, .blue = 175, .ansi_fg = 85, .ansi_bg = 85},
    {.red = 95, .green = 255, .blue = 215, .ansi_fg = 86, .ansi_bg = 86},
    {.red = 95, .green = 255, .blue = 255, .ansi_fg = 87, .ansi_bg = 87},
    {.red = 135, .green = 0, .blue = 0, .ansi_fg = 88, .ansi_bg = 88},
    {.red = 135, .green = 0, .blue = 95, .ansi_fg = 89, .ansi_bg = 89},
    {.red = 135, .green = 0, .blue = 135, .ansi_fg = 90, .ansi_bg = 90},
    {.red = 135, .green = 0, .blue = 175, .ansi_fg = 91, .ansi_bg = 91},
    {.red = 135, .green = 0, .blue = 215, .ansi_fg = 92, .ansi_bg = 92},
    {.red = 135, .green = 0, .blue = 255, .ansi_fg = 93, .ansi_bg = 93},
    {.red = 135, .green = 95, .blue = 0, .ansi_fg = 94, .ansi_bg = 94},
    {.red = 135, .green = 95, .blue = 95, .ansi_fg = 95, .ansi_bg = 95},
    {.red = 135, .green = 95, .blue = 135, .ansi_fg = 96, .ansi_bg = 96},
    {.red = 135, .green = 95, .blue = 175, .ansi_fg = 97, .ansi_bg = 97},
    {.red = 135, .green = 95, .blue = 215, .ansi_fg = 98, .ansi_bg = 98},
    {.red = 135, .green = 95, .blue = 255, .ansi_fg = 99, .ansi_bg = 99},
    {.red = 135, .green = 135, .blue = 0, .ansi_fg = 100, .ansi_bg = 100},
    {.red = 135, .green = 135, .blue = 95, .ansi_fg = 101, .ansi_bg = 101},
    {.red = 135, .green = 135, .blue = 135, .ansi_fg = 102, .ansi_bg = 102},
    {.red = 135, .green = 135, .blue = 175, .ansi_fg = 103, .ansi_bg = 103},
    {.red = 135, .green = 135, .blue = 215, .ansi_fg = 104, .ansi_bg = 104},
    {.red = 135, .green = 135, .blue = 255, .ansi_fg = 105, .ansi_bg = 105},
    {.red = 135, .green = 175, .blue = 0, .ansi_fg = 106, .ansi_bg = 106},
    {.red = 135, .green = 175, .blue = 95, .ansi_fg = 107, .ansi_bg = 107},
    {.red = 135, .green = 175, .blue = 135, .ansi_fg = 108, .ansi_bg = 108},
    {.red = 135, .green = 175, .blue = 175, .ansi_fg = 109, .ansi_bg = 109},
    {.red = 135, .green = 175, .blue = 215, .ansi_fg = 110, .ansi_bg = 110},
    {.red = 135, .green = 175, .blue = 255, .ansi_fg = 111, .ansi_bg = 111},
    {.red = 135, .green = 215, .blue = 0, .ansi_fg = 112, .ansi_bg = 112},
    {.red = 135, .green = 215, .blue = 95, .ansi_fg = 113, .ansi_bg = 113},
    {.red = 135, .green = 215, .blue = 135, .ansi_fg = 114, .ansi_bg = 114},
    {.red = 135, .green = 215, .blue = 175, .ansi_fg = 115, .ansi_bg = 115},
    {.red = 135, .green = 215, .blue = 215, .ansi_fg = 116, .ansi_bg = 116},
    {.red = 135, .green = 215, .blue = 255, .ansi_fg = 117, .ansi_bg = 117},
    {.red = 135, .green = 255, .blue = 0, .ansi_fg = 118, .ansi_bg = 118},
    {.red = 135, .green = 255, .blue = 95, .ansi_fg = 119, .ansi_bg = 119},
    {.red = 135, .green = 255, .blue = 135, .ansi_fg = 120, .ansi_bg = 120},
    {.red = 135, .green = 255, .blue = 175, .ansi_fg = 121, .ansi_bg = 121},
    {.red = 135, .green = 255, .blue = 215, .ansi_fg = 122, .ansi_bg = 122},
    {.red = 135, .green = 255, .blue = 255, .ansi_fg = 123, .ansi_bg = 123},
    {.red = 175, .green = 0, .blue = 0, .ansi_fg = 124, .ansi_bg = 124},
    {.red = 175, .green = 0, .blue = 95, .ansi_fg = 125, .ansi_bg = 125},
    {.red = 175, .green = 0, .blue = 135, .ansi_fg = 126, .ansi_bg = 126},
    {.red = 175, .green = 0, .blue = 175, .ansi_fg = 127, .ansi_bg = 127},
    {.red = 175, .green = 0, .blue = 215, .ansi_fg = 128, .ansi_bg = 128},
    {.red = 175, .green = 0, .blue = 255, .ansi_fg = 129, .ansi_bg = 129},
    {.red = 175, .green = 95, .blue = 0, .ansi_fg = 130, .ansi_bg = 130},
    {.red = 175, .green = 95, .blue = 95, .ansi_fg = 131, .ansi_bg = 131},
    {.red = 175, .green = 95, .blue = 135, .ansi_fg = 132, .ansi_bg = 132},
    {.red = 175, .green = 95, .blue = 175, .ansi_fg = 133, .ansi_bg = 133},
    {.red = 175, .green = 95, .blue = 215, .ansi_fg = 134, .ansi_bg = 134},
    {.red = 175, .green = 95, .blue = 255, .ansi_fg = 135, .ansi_bg = 135},
    {.red = 175, .green = 135, .blue = 0, .ansi_fg = 136, .ansi_bg = 136},
    {.red = 175, .green = 135, .blue = 95, .ansi_fg = 137, .ansi_bg = 137},
    {.red = 175, .green = 135, .blue = 135, .ansi_fg = 138, .ansi_bg = 138},
    {.red = 175, .green = 135, .blue = 175, .ansi_fg = 139, .ansi_bg = 139},
    {.red = 175, .green = 135, .blue = 215, .ansi_fg = 140, .ansi_bg = 140},
    {.red = 175, .green = 135, .blue = 255, .ansi_fg = 141, .ansi_bg = 141},
    {.red = 175, .green = 175, .blue = 0, .ansi_fg = 142, .ansi_bg = 142},
    {.red = 175, .green = 175, .blue = 95, .ansi_fg = 143, .ansi_bg = 143},
    {.red = 175, .green = 175, .blue = 135, .ansi_fg = 144, .ansi_bg = 144},
    {.red = 175, .green = 175, .blue = 175, .ansi_fg = 145, .ansi_bg = 145},
    {.red = 175, .green = 175, .blue = 215, .ansi_fg = 146, .ansi_bg = 146},
    {.red = 175, .green = 175, .blue = 255, .ansi_fg = 147, .ansi_bg = 147},
    {.red = 175, .green = 215, .blue = 0, .ansi_fg = 148, .ansi_bg = 148},
    {.red = 175, .green = 215, .blue = 95, .ansi_fg = 149, .ansi_bg = 149},
    {.red = 175, .green = 215, .blue = 135, .ansi_fg = 150, .ansi_bg = 150},
    {.red = 175, .green = 215, .blue = 175, .ansi_fg = 151, .ansi_bg = 151},
    {.red = 175, .green = 215, .blue = 215, .ansi_fg = 152, .ansi_bg = 152},
    {.red = 175, .green = 215, .blue = 255, .ansi_fg = 153, .ansi_bg = 153},
    {.red = 175, .green = 255, .blue = 0, .ansi_fg = 154, .ansi_bg = 154},
    {.red = 175, .green = 255, .blue = 95, .ansi_fg = 155, .ansi_bg = 155},
    {.red = 175, .green = 255, .blue = 135, .ansi_fg = 156, .ansi_bg = 156},
    {.red = 175, .green = 255, .blue = 175, .ansi_fg = 157, .ansi_bg = 157},
    {.red = 175, .green = 255, .blue = 215, .ansi_fg = 158, .ansi_bg = 158},
    {.red = 175, .green = 255, .blue = 255, .ansi_fg = 159, .ansi_bg = 159},
    {.red = 215, .green = 0, .blue = 0, .ansi_fg = 160, .ansi_bg = 160},
    {.red = 215, .green = 0, .blue = 95, .ansi_fg = 161, .ansi_bg = 161},
    {.red = 215, .green = 0, .blue = 135, .ansi_fg = 162, .ansi_bg = 162},
    {.red = 215, .green = 0, .blue = 175, .ansi_fg = 163, .ansi_bg = 163},
    {.red = 215, .green = 0, .blue = 215, .ansi_fg = 164, .ansi_bg = 164},
    {.red = 215, .green = 0, .blue = 255, .ansi_fg = 165, .ansi_bg = 165},
    {.red = 215, .green = 95, .blue = 0, .ansi_fg = 166, .ansi_bg = 166},
    {.red = 215, .green = 95, .blue = 95, .ansi_fg = 167, .ansi_bg = 167},
    {.red = 215, .green = 95, .blue = 135, .ansi_fg = 168, .ansi_bg = 168},
    {.red = 215, .green = 95, .blue = 175, .ansi_fg = 169, .ansi_bg = 169},
    {.red = 215, .green = 95, .blue = 215, .ansi_fg = 170, .ansi_bg = 170},
    {.red = 215, .green = 95, .blue = 255, .ansi_fg = 171, .ansi_bg = 171},
    {.red = 215, .green = 135, .blue = 0, .ansi_fg = 172, .ansi_bg = 172},
    {.red = 215, .green = 135, .blue = 95, .ansi_fg = 173, .ansi_bg = 173},
    {.red = 215, .green = 135, .blue = 135, .ansi_fg = 174, .ansi_bg = 174},
    {.red = 215, .green = 135, .blue = 175, .ansi_fg = 175, .ansi_bg = 175},
    {.red = 215, .green = 135, .blue = 215, .ansi_fg = 176, .ansi_bg = 176},
    {.red = 215, .green = 135, .blue = 255, .ansi_fg = 177, .ansi_bg = 177},
    {.red = 215, .green = 175, .blue = 0, .ansi_fg = 178, .ansi_bg = 178},
    {.red = 215, .green = 175, .blue = 95, .ansi_fg = 179, .ansi_bg = 179},
    {.red = 215, .green = 175, .blue = 135, .ansi_fg = 180, .ansi_bg = 180},
    {.red = 215, .green = 175, .blue = 175, .ansi_fg = 181, .ansi_bg = 181},
    {.red = 215, .green = 175, .blue = 215, .ansi_fg = 182, .ansi_bg = 182},
    {.red = 215, .green = 175, .blue = 255, .ansi_fg = 183, .ansi_bg = 183},
    {.red = 215, .green = 215, .blue = 0, .ansi_fg = 184, .ansi_bg = 184},
    {.red = 215, .green = 215, .blue = 95, .ansi_fg = 185, .ansi_bg = 185},
    {.red = 215, .green = 215, .blue = 135, .ansi_fg = 186, .ansi_bg = 186},
    {.red = 215, .green = 215, .blue = 175, .ansi_fg = 187, .ansi_bg = 187},
    {.red = 215, .green = 215, .blue = 215, .ansi_fg = 188, .ansi_bg = 188},
    {.red = 215, .green = 215, .blue = 255, .ansi_fg = 189, .ansi_bg = 189},
    {.red = 215, .green = 255, .blue = 0, .ansi_fg = 190, .ansi_bg = 190},
    {.red = 215, .green = 255, .blue = 95, .ansi_fg = 191, .ansi_bg = 191},
    {.red = 215, .green = 255, .blue = 135, .ansi_fg = 192, .ansi_bg = 192},
    {.red = 215, .green = 255, .blue = 175, .ansi_fg = 193, .ansi_bg = 193},
    {.red = 215, .green = 255, .blue = 215, .ansi_fg = 194, .ansi_bg = 194},
    {.red = 215, .green = 255, .blue = 255, .ansi_fg = 195, .ansi_bg = 195},
    {.red = 255, .green = 0, .blue = 0, .ansi_fg = 196, .ansi_bg = 196},
    {.red = 255, .green = 0, .blue = 95, .ansi_fg = 197, .ansi_bg = 197},
    {.red = 255, .green = 0, .blue = 135, .ansi_fg = 198, .ansi_bg = 198},
    {.red = 255, .green = 0, .blue = 175, .ansi_fg = 199, .ansi_bg = 199},
    {.red = 255, .green = 0, .blue = 215, .ansi_fg = 200, .ansi_bg = 200},
    {.red = 255, .green = 0, .blue = 255, .ansi_fg = 201, .ansi_bg = 201},
    {.red = 255, .green = 95, .blue = 0, .ansi_fg = 202, .ansi_bg = 202},
    {.red = 255, .green = 95, .blue = 95, .ansi_fg = 203, .ansi_bg = 203},
    {.red = 255, .green = 95, .blue = 135, .ansi_fg = 204, .ansi_bg = 204},
    {.red = 255, .green = 95, .blue = 175, .ansi_fg = 205, .ansi_bg = 205},
    {.red = 255, .green = 95, .blue = 215, .ansi_fg = 206, .ansi_bg = 206},
    {.red = 255, .green = 95, .blue = 255, .ansi_fg = 207, .ansi_bg = 207},
    {.red = 255, .green = 135, .blue = 0, .ansi_fg = 208, .ansi_bg = 208},
    {.red = 255, .green = 135, .blue = 95, .ansi_fg = 209, .ansi_bg = 209},
    {.red = 255, .green = 135, .blue = 135, .ansi_fg = 210, .ansi_bg = 210},
    {.red = 255, .green = 135, .blue = 175, .ansi_fg = 211, .ansi_bg = 211},
    {.red = 255, .green = 135, .blue = 215, .ansi_fg = 212, .ansi_bg = 212},
    {.red = 255, .green = 135, .blue = 255, .ansi_fg = 213, .ansi_bg = 213},
    {.red = 255, .green = 175, .blue = 0, .ansi_fg = 214, .ansi_bg = 214},
    {.red = 255, .green = 175, .blue = 95, .ansi_fg = 215, .ansi_bg = 215},
    {.red = 255, .green = 175, .blue = 135, .ansi_fg = 216, .ansi_bg = 216},
    {.red = 255, .green = 175, .blue = 175, .ansi_fg = 217, .ansi_bg = 217},
    {.red = 255, .green = 175, .blue = 215, .ansi_fg = 218, .ansi_bg = 218},
    {.red = 255, .green = 175, .blue = 255, .ansi_fg = 219, .ansi_bg = 219},
    {.red = 255, .green = 215, .blue = 0, .ansi_fg = 220, .ansi_bg = 220},
    {.red = 255, .green = 215, .blue = 95, .ansi_fg = 221, .ansi_bg = 221},
    {.red = 255, .green = 215, .blue = 135, .ansi_fg = 222, .ansi_bg = 222},
    {.red = 255, .green = 215, .blue = 175, .ansi_fg = 223, .ansi_bg = 223},
    {.red = 255, .green = 215, .blue = 215, .ansi_fg = 224, .ansi_bg = 224},
    {.red = 255, .green = 215, .blue = 255, .ansi_fg = 225, .ansi_bg = 225},
    {.red = 255, .green = 255, .blue = 0, .ansi_fg = 226, .ansi_bg = 226},
    {.red = 255, .green = 255, .blue = 95, .ansi_fg = 227, .ansi_bg = 227},
    {.red = 255, .green = 255, .blue = 135, .ansi_fg = 228, .ansi_bg = 228},
    {.red = 255, .green = 255, .blue = 175, .ansi_fg = 229, .ansi_bg = 229},
    {.red = 255, .green = 255, .blue = 215, .ansi_fg = 230, .ansi_bg = 230},
    {.red = 255, .green = 255, .blue = 255, .ansi_fg = 231, .ansi_bg = 231},
    {.red = 8, .green = 8, .blue = 8, .ansi_fg = 232, .ansi_bg = 232},
    {.red = 18, .green = 18, .blue = 18, .ansi_fg = 233, .ansi_bg = 233},
    {.red = 28, .green = 28, .blue = 28, .ansi_fg = 234, .ansi_bg = 234},
    {.red = 38, .green = 38, .blue = 38, .ansi_fg = 235, .ansi_bg = 235},
    {.red = 48, .green = 48, .blue = 48, .ansi_fg = 236, .ansi_bg = 236},
    {.red = 58, .green = 58, .blue = 58, .ansi_fg = 237, .ansi_bg = 237},
    {.red = 68, .green = 68, .blue = 68, .ansi_fg = 238, .ansi_bg = 238},
    {.red = 78, .green = 78, .blue = 78, .ansi_fg = 239, .ansi_bg = 239},
    {.red = 88, .green = 88, .blue = 88, .ansi_fg = 240, .ansi_bg = 240},
    {.red = 98, .green = 98, .blue = 98, .ansi_fg = 241, .ansi_bg = 241},
    {.red = 108, .green = 108, .blue = 108, .ansi_fg = 242, .ansi_bg = 242},
    {.red = 118, .green = 118, .blue = 118, .ansi_fg = 243, .ansi_bg = 243},
    {.red = 128, .green = 128, .blue = 128, .ansi_fg = 244, .ansi_bg = 244},
    {.red = 138, .green = 138, .blue = 138, .ansi_fg = 245, .ansi_bg = 245},
    {.red = 148, .green = 148, .blue = 148, .ansi_fg = 246, .ansi_bg = 246},
    {.red = 158, .green = 158, .blue = 158, .ansi_fg = 247, .ansi_bg = 247},
    {.red = 168, .green = 168, .blue = 168, .ansi_fg = 248, .ansi_bg = 248},
    {.red = 178, .green = 178, .blue = 178, .ansi_fg = 249, .ansi_bg = 249},
    {.red = 188, .green = 188, .blue = 188, .ansi_fg = 250, .ansi_bg = 250},
    {.red = 198, .green = 198, .blue = 198, .ansi_fg = 251, .ansi_bg = 251},
    {.red = 208, .green = 208, .blue = 208, .ansi_fg = 252, .ansi_bg = 252},
    {.red = 218, .green = 218, .blue = 218, .ansi_fg = 253, .ansi_bg = 253},
    {.red = 228, .green = 228, .blue = 228, .ansi_fg = 254, .ansi_bg = 254},
    {.red = 238, .green = 238, .blue = 238, .ansi_fg = 255, .ansi_bg = 255},
};

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

/*
 * Convert to 3-bit colour depth using a DAC.
 */
DacColour convert_3_4_bit_colour(const int num_colours, const Glyph glyph, const char *dac) {
	DacColour dac_colour = {0};

	for (int dac_index = 0; dac_index < NUM_DACS; dac_index++) {
		if (!strcmp(dac, DACS[dac_index].dac)) {
			int smallest_foreground_diff = COLOUR_255 * 3;
			int smallest_background_diff = COLOUR_255 * 3;

			for (int colour_index = 0; colour_index < num_colours; colour_index++) {
				DacColour candidate = DACS[dac_index].colours[colour_index];
				int foreground_diff = diff(glyph.fg_red, candidate.red) + diff(glyph.fg_green, candidate.green) +
				                      diff(glyph.fg_blue, candidate.blue);
				int background_diff = diff(glyph.bg_red, candidate.red) + diff(glyph.bg_green, candidate.green) +
				                      diff(glyph.bg_blue, candidate.blue);

				if (foreground_diff < smallest_foreground_diff) {
					smallest_foreground_diff = foreground_diff;

					dac_colour.ansi_fg = candidate.ansi_fg;
				}

				if (background_diff < smallest_background_diff) {
					smallest_background_diff = background_diff;

					dac_colour.ansi_bg = candidate.ansi_bg;
				}
			}
		}
	}

	return dac_colour;
}

/*
 * Convert to 8-bit colour depth using a DAC.
 */
DacColour convert_8_bit_colour(const Glyph glyph) {
	DacColour dac_colour = {0};
	int smallest_foreground_diff = COLOUR_255 * 3;
	int smallest_background_diff = COLOUR_255 * 3;

	for (int colour_index = 0; colour_index < NUM_DAC_8_BIT_COLOURS; colour_index++) {
		DacColour candidate = DAC_8_BIT_COLOURS[colour_index];
		int foreground_diff = diff(glyph.fg_red, candidate.red) + diff(glyph.fg_green, candidate.green) +
		                      diff(glyph.fg_blue, candidate.blue);
		int background_diff = diff(glyph.bg_red, candidate.red) + diff(glyph.bg_green, candidate.green) +
		                      diff(glyph.bg_blue, candidate.blue);

		if (foreground_diff < smallest_foreground_diff) {
			smallest_foreground_diff = foreground_diff;

			dac_colour.ansi_fg = candidate.ansi_fg;
		}

		if (background_diff < smallest_background_diff) {
			smallest_background_diff = background_diff;

			dac_colour.ansi_bg = candidate.ansi_bg;
		}
	}

	return dac_colour;
}

/*
 * Print usage information.
 */
void usage(const char *prog) {
	printf("Usage: %s FILE...\n"
	       "View image FILE in a terminal/console.\n"
	       "Example: %s dog.jpg house.png\n\n"
	       "Options:\n"
	       "  -c, --colour-depth  output colour depth (" COLOUR_DEPTH_3 ", " COLOUR_DEPTH_4 ", " COLOUR_DEPTH_8
	       ", " COLOUR_DEPTH_24 ") (default: " DEFAULT_COLOUR_DEPTH ")\n"
	       "  -d, --dac           DAC used for 3-bit or 4-bit colour depth\n"
	       "                      (" DAC_VGA ", " DAC_WIN_XP ", " DAC_POWERSHELL ", " DAC_VS_CODE ", " DAC_WIN_10
	       ", " DAC_TERMINAL_APP ",\n"
	       "                      " DAC_PUTTY ", " DAC_MIRC ", " DAC_XTERM ", " DAC_LINUX ", " DAC_ECLIPSE
	       ") (default: " DEFAULT_DAC ")\n"
	       "  -g, --greyscale     output in greyscale\n",
	       prog,
	       prog);
}

int main(int argc, char *argv[]) {
	static char opt_greyscale = 0;
	static struct option long_opts[] = {{"help", no_argument, NULL, 'h'},
	                                    {"colour-depth", required_argument, NULL, 'c'},
	                                    {"dac", required_argument, NULL, 'd'},
	                                    {"greyscale", no_argument, NULL, 'g'},
	                                    {NULL, 0, NULL, 0}};
	int ch = 0;
	int images = 0;
	char *colour_depth = DEFAULT_COLOUR_DEPTH;
	char *dac = DEFAULT_DAC;

	while ((ch = getopt_long(argc, argv, "c:d:gh", long_opts, NULL)) != -1) {
		switch (ch) {
			case 'c':
				if (strcmp(optarg, COLOUR_DEPTH_3) && strcmp(optarg, COLOUR_DEPTH_4) &&
				    strcmp(optarg, COLOUR_DEPTH_8) && strcmp(optarg, COLOUR_DEPTH_24)) {
					fprintf(stderr, "%s: invalid colour depth '%s'\n", argv[0], optarg);

					return EXIT_FAILURE;
				}

				colour_depth = optarg;
				argv[optind - 2] = NULL;
				argv[optind - 1] = NULL;

				break;

			case 'd':
				if (strcmp(optarg, DAC_VGA) && strcmp(optarg, DAC_WIN_XP) && strcmp(optarg, DAC_POWERSHELL) &&
				    strcmp(optarg, DAC_VS_CODE) && strcmp(optarg, DAC_WIN_10) && strcmp(optarg, DAC_TERMINAL_APP) &&
				    strcmp(optarg, DAC_PUTTY) && strcmp(optarg, DAC_MIRC) && strcmp(optarg, DAC_XTERM) &&
				    strcmp(optarg, DAC_LINUX) && strcmp(optarg, DAC_ECLIPSE)) {
					fprintf(stderr, "%s: invalid DAC '%s'\n", argv[0], optarg);

					return EXIT_FAILURE;
				}

				dac = optarg;
				argv[optind - 2] = NULL;
				argv[optind - 1] = NULL;

				break;

			case 'g':
				opt_greyscale = 1;
				argv[optind - 1] = NULL;

				break;

			case 'h':
				usage(argv[0]);

				return EXIT_SUCCESS;

			default:
				return EXIT_FAILURE;
		}
	}

	if (argc <= 1) {
		fprintf(stderr, "No input file specified.\n");

		return EXIT_FAILURE;
	}

	MagickWandGenesis();

	for (int i = 1; i < argc; i++) {
		if (argv[i] == NULL) {
			continue;
		}

		MagickWand *wand = NewMagickWand();
		MagickBooleanType status = MagickReadImage(wand, argv[i]);

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

		if (images++ > 0) {
			printf("\n");
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
						unsigned int red = COLOUR_255 * PixelGetRed(row[block_x]);
						unsigned int green = COLOUR_255 * PixelGetGreen(row[block_x]);
						unsigned int blue = COLOUR_255 * PixelGetBlue(row[block_x]);

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

				// Convert to greyscale, if required.
				if (opt_greyscale == 1) {
					convert_to_greyscale(block[index]);
				}

				// Convert to lower colour depth using specified DAC, if required.
				if (!strcmp(colour_depth, COLOUR_DEPTH_3)) {
					DacColour colour = convert_3_4_bit_colour(NUM_DAC_3_BIT_COLOURS, block[index], dac);

					printf("\x1b[%d;%dm%s\x1b[0m", colour.ansi_fg, colour.ansi_bg, glyph);
				} else if (!strcmp(colour_depth, COLOUR_DEPTH_4)) {
					DacColour colour = convert_3_4_bit_colour(NUM_DAC_4_BIT_COLOURS, block[index], dac);

					printf("\x1b[%d;%dm%s\x1b[0m", colour.ansi_fg, colour.ansi_bg, glyph);
				} else if (!strcmp(colour_depth, COLOUR_DEPTH_8)) {
					DacColour colour = convert_8_bit_colour(block[index]);

					printf("\x1b[38;5;%d;48;5;%dm%s\x1b[0m", colour.ansi_fg, colour.ansi_bg, glyph);
				} else {
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
			}

			printf("\n");
		}

		wand = DestroyMagickWand(wand);
	}

	MagickWandTerminus();

	return EXIT_SUCCESS;
}
