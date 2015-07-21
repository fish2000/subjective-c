// Copyright 2015 Alexander Bohn <fish2000@gmail.com>
// License: MIT (see COPYING.MIT file)

#ifndef LPC_APPLE_HH_INCLUDE_GUARD_
#define LPC_APPLE_HH_INCLUDE_GUARD_

#ifdef __OBJC__
#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>
#import <CoreFoundation/CoreFoundation.h>
#endif

#include <memory>
#include <cstdio>
#include <cstring>

#include <libimread/libimread.hpp>
#include <libimread/image.hh>
#include <libimread/imageformat.hh>

namespace im {
    
    #ifdef __OBJC__
    namespace cf {
        
        /// NOTE TO SELF: replace this with CF++ and the like;
        /// see also objc-rt.hh
        
        template <typename C = CFTypeRef>
        struct unref {
            constexpr unref() noexcept = default;
            template <typename U> unref(const unref<U>&) noexcept {};
            void operator()(C ref) { if (ref) CFRelease(ref); }
        };
        
        template <typename Ref>
        using REF = std::unique_ptr<typename std::decay<Ref>::type, unref<Ref>>;
        
    }
    #endif
    
    class NSImageFormat : public ImageFormat {
        
        public:
            typedef std::true_type can_read;
            
            virtual std::unique_ptr<Image> read(byte_source *src,
                                                ImageFactory *factory,
                                                const options_map &opts) override;
    };
    
    namespace format {
        using NS = NSImageFormat;
        using Apple = NSImageFormat;
    }

}

#endif // LPC_APPLE_HH_INCLUDE_GUARD_