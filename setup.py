
from distutils.core import setup

setup (name = 'gfx',
       version = '1.0',
       description = 'This is a test package',
       # hmm, puts dlls in site-packages. messy
       data_files = [('Lib\site-packages', [
                        'bin/gfx.pyd',
                        'bin/libjpeg-9.dll',
                        'bin/libpng16-16.dll',
                        'bin/libtiff-5.dll',
                        'bin/libwebp-7.dll',
                        'bin/SDL2.dll',
                        'bin/SDL2_image.dll',
                        'bin/zlib1.dll'
                        ]
                     )])