// Copyright 2012 Luis Pedro Coelho <luis@luispedro.org>
// License: MIT (see COPYING.MIT file)

#ifndef LPC_WEBP_H_INCLUDE_GUARD_
#define LPC_WEBP_H_INCLUDE_GUARD_

#include <libimread/libimread.hpp>

namespace im {
    
    class WebPFormat : public ImageFormat {
        public:
            typedef std::true_type can_read;
            
            std::unique_ptr<Image> read(byte_source *src,
                                        ImageFactory *factory,
                                        const options_map &opts);
    };
    
    namespace format {
        using WebP = WebPFormat;
    }
    
}


#endif // LPC_WEBP_H_INCLUDE_GUARD_
