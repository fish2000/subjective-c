// Copyright 2012 Luis Pedro Coelho <luis@luispedro.org>
// License: MIT (see COPYING.MIT file)

#include <libimread/IO/apple.hh>

namespace im {

    namespace ns {
        
        std::unique_ptr<Image> IMDecodeDataVector(std::vector<byte> data,
                                                  ImageFactory *factory) {
            @autoreleasepool {
                NSData *datum;
                NSBitmapImageRep *rep;
                NSBitmapFormat format;
                
                @autoreleasepool {
                    datum = [[NSData alloc] initWithBytes:(const void *)&data[0]
                                                   length:(NSInteger)data.size()];
                    rep = [[NSBitmapImageRep alloc] initWithData:datum];
                    format = [rep bitmapFormat];
                };
                #if !__has_feature(objc_arc)
                    [datum release];
                #endif
                
                NSInteger height = [rep pixelsHigh];
                NSInteger width = [rep pixelsWide];
                NSInteger channels = [rep samplesPerPixel];
                int bps = (int)[rep bitsPerSample];
                int siz = (bps / 8) + bool(bps % 8);
                
                std::unique_ptr<Image> output(factory->create(
                    bps, height, width, channels));
                
                if (format & NSFloatingPointSamplesBitmapFormat) {
                    float *frowp = output->rowp_as<float>(0);
                    std::memcpy(frowp, (float *)[rep bitmapData], siz*height*width*channels);
                } else {
                    byte *rowp = output->rowp_as<byte>(0);
                    std::memcpy(rowp, static_cast<byte*>([rep bitmapData]), siz*height*width*channels);
                }
                #if !__has_feature(objc_arc)
                    [rep release];
                #endif
                return output;
            };
        }
        
    }
    
    std::unique_ptr<Image> NSImageFormat::read(byte_source *src,
                                               ImageFactory *factory,
                                               const options_map &opts)  {
        std::vector<byte> data = src->full_data();
        @autoreleasepool {
            return ns::IMDecodeDataVector(data, factory);
        };
    }
    
}