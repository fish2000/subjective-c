/// Copyright 2014 Alexander Böhn <fish2000@gmail.com>
/// License: MIT (see COPYING.MIT file)

#include <cstring>
#include <libimread/libimread.hpp>
#include <libimread/ext/categories/NSBitmapImageRep+IM.hh>
#include <libimread/objc-rt/types.hh>

using im::byte;
using im::Image;
using im::ImageFactory;

#ifdef __OBJC__

@implementation NSBitmapImageRep (AXBitmapImageRepAdditions)

+ (instancetype) imageRepWithByteVector:(std::vector<byte> const&)byteVector {
    NSBitmapImageRep* rep;
    @autoreleasepool {
        NSData* datum;
        datum = [[NSData alloc] initWithBytes:(const void*)&byteVector[0]
                                       length:(NSInteger)byteVector.size()];
        rep = [[NSBitmapImageRep alloc] initWithData:datum];
    };
    return rep;
}

+ (instancetype) imageRepWithImage:(Image const&)image {
    NSBitmapImageRep* rep = [[NSBitmapImageRep alloc] initWithImage:image];
    return rep;
}

- initWithByteVector:(std::vector<byte> const&)byteVector {
    @autoreleasepool {
        NSData* datum;
        datum = [[NSData alloc] initWithBytes:(const void*)&byteVector[0]
                                       length:(NSInteger)byteVector.size()];
        [self initWithData:datum];
    }
    return self;
}

- initWithImage:(Image const&)image {
    BOOL alpha = objc::boolean(image.ndims() > 2 ? image.dim(2) > 3 : false);
    NSInteger width = (NSInteger)image.dim(0);
    NSInteger height = (NSInteger)image.dim(1);
    NSInteger channels = (NSInteger)(image.ndims() > 2 ? image.dim(2) : 1);
    NSInteger bps = (NSInteger)image.nbits();
    int siz = (bps / 8) + bool(bps % 8);
    
    /// Pass `nil` to make NSBitmapImageRep do right by its own allocations:
    /// q.v. http://stackoverflow.com/a/16097891/298171
    /// …and http://stackoverflow.com/a/20526575/298171
    [self initWithBitmapDataPlanes:nil
                        pixelsWide:width
                        pixelsHigh:height
                     bitsPerSample:bps
                   samplesPerPixel:channels
                          hasAlpha:alpha
                          isPlanar:NO
                    colorSpaceName:NSCalibratedRGBColorSpace
                      bitmapFormat:NSAlphaNonpremultipliedBitmapFormat
                       bytesPerRow:(NSInteger)image.dim(0) * (image.ndims() > 2 ? image.dim(2) : 1)
                      bitsPerPixel:(NSInteger)(image.ndims() > 2 ? image.dim(2) * 8 : 8)];
    
    /// Manually copy the image buffer to [self bitmapData] --
    std::memcpy([self bitmapData], image.rowp_as<byte * _Nullable>(0),
                                   siz*height*width*channels);
    return self;
}

- (std::unique_ptr<Image>) imageUsingImageFactory:(ImageFactory*)factory {
    NSInteger height = [self pixelsHigh];
    NSInteger width = [self pixelsWide];
    NSInteger channels = [self samplesPerPixel];
    int bps = (int)[self bitsPerSample];
    int siz = (bps / 8) + bool(bps % 8);
    
    std::unique_ptr<Image> output(factory->create(
        bps, height, width, channels));
    
    if ([self bitmapFormat] & NSFloatingPointSamplesBitmapFormat) {
        float* frowp = output->rowp_as<float>(0);
        std::memcpy(frowp, reinterpret_cast<float*>([self bitmapData]),
                           siz*height*width*channels);
    } else {
        byte* irowp = output->rowp_as<byte>(0);
        std::memcpy(irowp, static_cast<byte*>([self bitmapData]),
                           siz*height*width*channels);
    }
    
    return output;
}

@end

#endif /// __OBJC__