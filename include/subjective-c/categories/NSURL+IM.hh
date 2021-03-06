/// Copyright 2012-2015 Alexander Bohn <fish2000@gmail.com>
/// License: MIT (see COPYING.MIT file)

#ifndef LIBIMREAD_EXT_CATEGORIES_NSURL_PLUS_IM_HH_
#define LIBIMREAD_EXT_CATEGORIES_NSURL_PLUS_IM_HH_

#include <string>
#include <subjective-c/subjective-c.hpp>
#import  <AppKit/AppKit.h>
#import  <Foundation/Foundation.h>

constexpr static const NSBitmapImageFileType AXPVRFileType = static_cast<NSBitmapImageFileType>(444);

namespace filesystem {
    class path;
}

namespace objc {
    
    namespace image {
        
        template <NSBitmapImageFileType t>
        struct suffix_t;
        
        #define DEFINE_SUFFIX(endstring, nstype)                                        \
        template <>                                                                     \
        struct suffix_t<nstype> {                                                       \
            constexpr static const std::size_t N = objc::static_strlen(endstring);      \
            constexpr static const char endstr[N] = endstring;                          \
            static const NSBitmapImageFileType typecode = nstype;                       \
        };
        
        DEFINE_SUFFIX("tiff", NSTIFFFileType);
        DEFINE_SUFFIX("bmp",  NSBMPFileType);
        DEFINE_SUFFIX("gif",  NSGIFFileType);
        DEFINE_SUFFIX("jpg",  NSJPEGFileType);
        DEFINE_SUFFIX("png",  NSPNGFileType);
        DEFINE_SUFFIX("jp2",  NSJPEG2000FileType);
        DEFINE_SUFFIX("pvr",  AXPVRFileType);
        
        template <NSBitmapImageFileType nstype>
        constexpr char const* suffix_value = suffix_t<nstype>::endstr;
        
        extern std::string suffix(NSBitmapImageFileType nstype);
        extern NSInteger filetype(std::string const& suffix);
        
    };
    
};

@interface NSURL (AXURLAdditions)
+ (instancetype)            fileURLWithFilesystemPath:(filesystem::path const&)path;
-                           initFileURLWithFilesystemPath:(filesystem::path const&)path;
- (instancetype)            URLByAppendingSTLPathComponent:(std::string const&)component;
- (instancetype)            URLByAppendingFilesystemPath:(filesystem::path const&)path;
- (BOOL)                    openWithApplication:(NSString*)applicationName;
- (BOOL)                    preview;
- (BOOL)                    isImage;
- (NSBitmapImageFileType)   imageFileType;
- (filesystem::path)        filesystemPath;
- (std::string)             STLString;
@end

#endif /// LIBIMREAD_EXT_CATEGORIES_NSURL_PLUS_IM_HH_