/// Copyright 2012-2015 Alexander Bohn <fish2000@gmail.com>
/// License: MIT (see COPYING.MIT file)

#ifndef LIBIMREAD_EXT_CATEGORIES_NSBITMAPIMAGEREP_PLUS_IM_HH_
#define LIBIMREAD_EXT_CATEGORIES_NSBITMAPIMAGEREP_PLUS_IM_HH_

#include <memory>
#include <subjective-c/subjective-c.hpp>
#import  <Foundation/Foundation.h>

using objc::byte;
using objc::bytevec_t;

namespace im {
    class Image;
    class ImageFactory;
}

using im::Image;
using im::ImageFactory;

@interface NSBitmapImageRep (AXBitmapImageRepAdditions)

+ (instancetype)           imageRepWithByteVector:(bytevec_t const&)byteVector;
+ (instancetype)           imageRepWithImage:(Image const&)image;
-                          initWithByteVector:(bytevec_t const&)byteVector;
-                          initWithImage:(Image const&)image;
- (std::unique_ptr<Image>) imageUsingImageFactory:(ImageFactory*)factory;

@end

#endif /// LIBIMREAD_EXT_CATEGORIES_NSBITMAPIMAGEREP_PLUS_IM_HH_