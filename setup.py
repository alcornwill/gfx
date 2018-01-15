
from distutils.core import setup

setup(
    name = 'gfx',
    version = '1.0',
    description = 'This is a test package',
    data_files = [(
        # hmm, puts dlls in site-packages. messy
        'Lib\site-packages', [
            'bin/gfx.pyd',
            'bin/SDL2.dll',
            'bin/SDL2_image.dll',
        ])]
    )