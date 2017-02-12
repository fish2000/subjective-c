/// Copyright 2012-2015 Alexander Bohn <fish2000@gmail.com>
/// License: MIT (see COPYING.MIT file)

#ifndef LIBIMREAD_EXT_CATEGORIES_NSBITMAPIMAGEREP_PLUS_IM_HH_
#define LIBIMREAD_EXT_CATEGORIES_NSBITMAPIMAGEREP_PLUS_IM_HH_

#include <vector>
#include <memory>
#import  <Foundation/Foundation.h>
#import  <Cocoa/Cocoa.h>
#include <libimread/libimread.hpp>
#include <libimread/image.hh>

using im::byte;
using im::Image;
using im::ImageFactory;

@interface NSBitmapImageRep (AXBitmapImageRepAdditions)
+ (instancetype)           imageRepWithByteVector:(std::vector<byte> const&)byteVector;
+ (instancetype)           imageRepWithImage:(Image const&)image;
-                          initWithByteVector:(std::vector<byte> const&)byteVector;
-                          initWithImage:(Image const&)image;
- (std::unique_ptr<Image>) imageUsingImageFactory:(ImageFactory*)factory;
@end

#endif /// LIBIMREAD_EXT_CATEGORIES_NSBITMAPIMAGEREP_PLUS_IM_HH_