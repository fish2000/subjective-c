
from __future__ import print_function

from unittest2 import TestCase
import sys
from os import listdir
from os.path import join, abspath, expanduser, dirname

class BaseCase(TestCase):
    
    def setUp(self):
        self._image_paths = map(
            lambda nm: expanduser(join(dirname(dirname(dirname(dirname(__file__)))), 'tests', 'data', nm)),
                listdir(abspath(expanduser(join(dirname(dirname(dirname(dirname(__file__)))), 'tests', 'data'))))
                )
        self.image_paths = set([pth for pth in self._image_paths if pth.lower().endswith('jpg')])
        self.jpgs = set([pth for pth in self._image_paths if pth.lower().endswith('jpg')])
        self.pngs = set([pth for pth in self._image_paths if pth.lower().endswith('png')])
        self.tifs = set([pth for pth in self._image_paths if pth.lower().endswith('tif')])
        self.pvrs = set([pth for pth in self._image_paths if pth.lower().endswith('pvr')])
        

def main(discover=False):
    import nose2
    if discover:
        return nose2.discover()
    else:
        return nose2.main()

if __name__ == '__main__':
    sys.exit(main())