project('sallyview', 'c',
	version: '0.1',
	default_options: ['warning_level=3'])

imageMagick = dependency('MagickWand', version: '>=7.0.0')

executable('sallyview',
	'main.c',
	dependencies: imageMagick,
	install: true)
