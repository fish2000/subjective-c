/// Copyright 2016 Alexander Böhn <fish2000@gmail.com>.org>
/// License: MIT (see COPYING.MIT file)

#ifndef LIBIMREAD_IO_TGA_HH_
#define LIBIMREAD_IO_TGA_HH_

#include <libimread/libimread.hpp>
#include <libimread/base.hh>

namespace im {
    
    class TGAFormat : public ImageFormatBase<TGAFormat> {
        
        public:
            using can_read = std::true_type;
            using can_write = std::true_type;
            
            DECLARE_OPTIONS(
                _signatures = {
                    SIGNATURE("\x42\x4d", 2)
                },
                _suffixes = { "tga", "tpic" },
                _mimetype = "image/x-tga"
            );
            
            virtual std::unique_ptr<Image> read(byte_source* src,
                                                ImageFactory* factory,
                                                const options_map& opts) override;
            
            virtual void write(Image& input,
                               byte_sink* output,
                               const options_map& opts) override;
    
    };
    
    namespace format {
        using TGA = TGAFormat;
    }
    
}

#endif /// LIBIMREAD_IO_TGA_HH_
