
from __future__ import print_function
from basecase import BaseCase

import im

class NumpyImageTests(BaseCase):
    
    def test_load_image_from_filename(self):
        for image_path in self.image_paths:
            image = im.NumpyImage(image_path)
            self.assertIsNotNone(image)
    