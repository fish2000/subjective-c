/// Copyright 2014 Alexander Böhn <fish2000@gmail.com>
/// License: MIT (see COPYING.MIT file)

#ifndef LIBIMREAD_IO_PNG_HH_
#define LIBIMREAD_IO_PNG_HH_

#include <cstring>
#include <csetjmp>
#include <vector>
#include <sstream>

#include <libimread/libimread.hpp>
#include <libimread/base.hh>
#include <libimread/options.hh>

namespace im {
    
    class PNGFormat : public ImageFormat {
        
        public:
            using can_read = std::true_type;
            using can_write = std::true_type;
            
            DECLARE_OPTIONS(
                "\x89\x50\x4E\x47\x0D\x0A\x1A\x0A",
                "png",
                "image/png");
            
            virtual std::unique_ptr<Image> read(byte_source* src,
                                                ImageFactory* factory,
                                                const options_map& opts) override;
            virtual void write(Image& input,
                               byte_sink* output,
                               const options_map& opts) override;
            
            /// NOT AN OVERRIDE:
            virtual void write_ios(Image& input,
                                   byte_sink* output,
                                   const options_map& opts);
    };
    
    namespace format {
        using PNG = PNGFormat;
    }
    
}


#endif /// LIBIMREAD_IO_PNG_HH_
