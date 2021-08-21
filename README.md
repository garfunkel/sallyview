# sallyview
A terminal/console viewer for image files.

Sallyview reads images as 4x8 pixel regions and displays the results using Unicode [block elements](https://en.wikipedia.org/wiki/Block_Elements).

## Usage
	Usage: sallyview FILE...
	View image FILE in a terminal/console.
	Example: sallyview dog.jpg house.png

	Options:
	-c, --colour-depth  output colour depth (3, 4, 8, 24) (default: 24)
	-d, --dac           DAC used for 3-bit or 4-bit colour depth
	                    (vga, win-xp, powershell, vs-code, win-10, terminal.app,
	                    putty, mirc, xterm, linux, eclipse) (default: terminal.app)
	-g, --greyscale     output in greyscale

## Compatibility
Sallyview should support most platforms that have ImageMagick available. It has been tested on:
* Arch Linux
* CentOS
* MacOS

## Dependencies
Sallyview only requires ImageMagick to be installed.

## Building
Meson is used to build Sallyview. Build by running the following commands:

	$ cd sallyview
	$ meson build && cd build
	$ meson compile

## Colour Depths
Sallyview supports several colour depths:
* [3-bit](https://en.wikipedia.org/wiki/ANSI_escape_code#3-bit_and_4-bit) - 'standard' colours
* [4-bit](https://en.wikipedia.org/wiki/ANSI_escape_code#3-bit_and_4-bit) - 'standard' colours + 'bright' colours
* [8-bit](https://en.wikipedia.org/wiki/ANSI_escape_code#8-bit) - 256 colours
* [24-bit](https://en.wikipedia.org/wiki/ANSI_escape_code#24-bit) - true colour

To support colour depth conversion from the original image, emulation of digital-to-analogue converters (DACs) is required. Each of these DACs interprets colours somewhat differently. Numerous DACs have been built in to Sallyview and are available as options:
* VGA
* Windows XP Console
* Windows PowerShell
* Visual Studio Code
* Windows 10 Console
* Terminal.app
* PuTTY
* mIRC
* xterm
* Linux
* Eclipse

In addition, Sallyview also supports rendering images in greyscale.