
from __future__ import print_function
from basecase import BaseCase

class PlanarOperationTests(BaseCase):
    
    def test_jpg_check_split(self):
        ''' Load some JPG files, split into plane image tuple,
            compare plane features (size etc) to image features '''
        for ImageType in self.imagetypes:
            for image_path in self.jpgs:
                image = ImageType(image_path)
                self.assertIsNotNone(image)
                planes = image.split()
                self.assertEqual(int(image.planes), len(planes))
                for plane in planes:
                    self.assertEqual(plane.width,  image.width)
                    self.assertEqual(plane.height, image.height)
                    self.assertEqual(plane.planes, 1)
    
    def test_jpg_check_split_merge(self):
        ''' Load some JPG files, split into plane image tuple,
            compare plane features (size etc) to image features,
            re-merge into composite image '''
        for ImageType in self.imagetypes:
            for image_path in self.jpgs:
                image = ImageType(image_path)
                self.assertIsNotNone(image)
                planes = image.split()
                self.assertEqual(int(image.planes), len(planes))
                for plane in planes:
                    self.assertEqual(plane.width,  image.width)
                    self.assertEqual(plane.height, image.height)
                    self.assertEqual(plane.planes, 1)
                image2 = ImageType.merge(planes)
                self.assertEqual(image.width,  image2.width)
                self.assertEqual(image.height, image2.height)
                self.assertEqual(image.planes, image2.planes)
                self.assertEqual(image.buffer.tostring(),
                                 image2.buffer.tostring())
    