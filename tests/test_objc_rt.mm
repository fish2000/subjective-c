
#include <cstdlib>
#include <string>
#import <Cocoa/Cocoa.h>

#include <libimread/libimread.hpp>
#include <libimread/ext/filesystem/path.h>
#include <libimread/objc-rt.hh>

#include "include/catch.hpp"

namespace {
    
    TEST_CASE("[objc-rt] Send a message via objc::msg::send()", "[objc-rt-msg-send]") {
        filesystem::NamedTemporaryFile temporary;
        NSData *datum;
        NSURL *url;
        NSString *urlpath;
        std::string path;
        //std::string prefix = "file://";
        std::string prefix = "";
        std::size_t nbytes = 20 * 1024; /// 20480
        unsigned char randos[20480] = {0};
        
        @autoreleasepool {
            ::arc4random_buf(static_cast<void*>(randos), nbytes);
            path = prefix + temporary.str();
            datum = [[NSData alloc] initWithBytes:(const void *)&randos[0]
                                           length:(NSInteger)nbytes];
            urlpath = [[NSString alloc] initWithUTF8String:path.c_str()];
            url = [[NSURL alloc] initWithString:urlpath];
            
            objc::msg::send((id)datum,
                objc::selector("writeToFile:atomically:"),
                urlpath, YES);
            
            #if !__has_feature(objc_arc)
                [datum release];
                [urlpath release];
                [url release];
            #endif
        };
        
    }
    
}