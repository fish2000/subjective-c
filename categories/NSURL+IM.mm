/// Copyright 2014 Alexander Böhn <fish2000@gmail.com>
/// License: MIT (see COPYING.MIT file)

#include <subjective-c/subjective-c.hpp>
#include <libimread/ext/filesystem/path.h>
#import  <subjective-c/categories/NSString+STL.hh>
#import  <subjective-c/categories/NSURL+IM.hh>

namespace objc {
    
    namespace image {
        
        DECLARE_CONSTEXPR_CHAR(suffix_t<NSTIFFFileType>::endstr,       "tiff");
        DECLARE_CONSTEXPR_CHAR(suffix_t<NSBMPFileType>::endstr,        "bmp");
        DECLARE_CONSTEXPR_CHAR(suffix_t<NSGIFFileType>::endstr,        "gif");
        DECLARE_CONSTEXPR_CHAR(suffix_t<NSJPEGFileType>::endstr,       "jpg");
        DECLARE_CONSTEXPR_CHAR(suffix_t<NSPNGFileType>::endstr,        "png");
        DECLARE_CONSTEXPR_CHAR(suffix_t<NSJPEG2000FileType>::endstr,   "jp2");
        DECLARE_CONSTEXPR_CHAR(suffix_t<AXPVRFileType>::endstr,        "pvr");
        
        std::string suffix(NSBitmapImageFileType nstype) {
            // switch (nstype) {
            //     case NSTIFFFileType:        { return suffix_value<NSTIFFFileType>;     }
            //     case NSJPEGFileType:        { return suffix_value<NSJPEGFileType>;     }
            //     case NSPNGFileType:         { return suffix_value<NSPNGFileType>;      }
            //     case NSGIFFileType:         { return suffix_value<NSGIFFileType>;      }
            //     case NSBMPFileType:         { return suffix_value<NSBMPFileType>;      }
            //     case NSJPEG2000FileType:    { return suffix_value<NSJPEG2000FileType>; }
            //     case AXPVRFileType:         { return suffix_value<AXPVRFileType>;      }
            //     default:                    { return "";                               }
            // }
            switch (nstype) {
                case NSTIFFFileType:        { return "tiff";    }
                case NSJPEGFileType:        { return "jpg";     }
                case NSPNGFileType:         { return "png";     }
                case NSGIFFileType:         { return "gif";     }
                case NSBMPFileType:         { return "bmp";     }
                case NSJPEG2000FileType:    { return "jp2";     }
                case AXPVRFileType:         { return "pvr";     }
                default:                    { return "";        }
            }
        }
        
        NSInteger filetype(std::string const& suffix) {
            if (suffix == "tiff" || suffix == ".tiff" ||
                suffix == "tif"  || suffix == ".tif") {
                return static_cast<NSInteger>(NSTIFFFileType);
            } else if (suffix == "bmp"   || suffix == ".bmp") {
                return static_cast<NSInteger>(NSBMPFileType);
            } else if (suffix == "gif"   || suffix == ".gif") {
                return static_cast<NSInteger>(NSGIFFileType);
            } else if (suffix == "jpg"   || suffix == ".jpg" ||
                       suffix == "jpeg"  || suffix == ".jpeg") {
                return static_cast<NSInteger>(NSJPEGFileType);
            } else if (suffix == "png"   || suffix == ".png") {
                return static_cast<NSInteger>(NSPNGFileType);
            } else if (suffix == "jp2"   || suffix == ".jp2") {
                return static_cast<NSInteger>(NSJPEG2000FileType);
            } else if (suffix == "pvr"   || suffix == ".pvr" ||
                       suffix == "pvrtc" || suffix == ".pvrtc") {
                return static_cast<NSInteger>(AXPVRFileType);
            } else {
                /// NO MATCH
                return -1;
            }
        }
        
    }
}

@implementation NSURL (AXURLAdditions)

+ (instancetype) fileURLWithFilesystemPath:(filesystem::path const&)path {
    return [NSURL fileURLWithPath:[NSString
              stringWithSTLString:path.make_absolute().str()]
                      isDirectory:objc::boolean(path.is_directory())];
}

- initFileURLWithFilesystemPath:(filesystem::path const&)path {
    return [self initFileURLWithPath:[NSString
                 stringWithSTLString:path.make_absolute().str()]
                         isDirectory:objc::boolean(path.is_directory())];
}

- (instancetype) URLByAppendingSTLPathComponent:(std::string const&)component {
    return [self URLByAppendingPathComponent:[NSString stringWithSTLString:component]];
}

- (instancetype) URLByAppendingFilesystemPath:(filesystem::path const&)path {
    NSURL* url = [self copy];
    for (auto const& component : path.components()) {
        url = [url URLByAppendingSTLPathComponent:component];
    }
    return url;
}

- (BOOL) openWithApplication:(NSString*)applicationName {
    NSString* filePath = [[NSString alloc] initWithUTF8String:[self fileSystemRepresentation]];
    return [[NSWorkspace sharedWorkspace] openFile:filePath
                                   withApplication:applicationName];
}

- (BOOL) preview {
    return [self openWithApplication:@"Preview.app"];
}

- (BOOL) isImage {
    return objc::boolean(
        objc::image::filetype(
            [[self.pathExtension lowercaseString] STLString]) != -1);
}

- (NSBitmapImageFileType) imageFileType {
    return static_cast<NSBitmapImageFileType>(
        objc::image::filetype(
            [[self.pathExtension lowercaseString] STLString]));
}

- (filesystem::path) filesystemPath {
    filesystem::path out([self fileSystemRepresentation]);
    return out;
}

- (std::string) STLString {
    return [self fileSystemRepresentation];
}

@end
