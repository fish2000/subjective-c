
#include <algorithm>
#include <unordered_map>

#include <subjective-c/subjective-c.hpp>
#include <subjective-c/appkit.hh>
#import  <subjective-c/categories/NSURL+IM.hh>
#include <libimread/ext/filesystem/path.h>

#include "include/test_data.hpp"
#include "include/catch.hpp"

namespace {
    
    using filesystem::path;
    
    TEST_CASE("[appkit-copy-paste] Copy and paste PNG image data",
              "[appkit-copy-paste-png-image-data]")
    {
        path basedir(im::test::basedir);
        const std::vector<path> pngs = basedir.list("*.png");
        
        @autoreleasepool {
            
            std::vector<NSPasteboard*> boards;
            std::unordered_map<path, objc::object<NSImage>> images;
            
            std::for_each(pngs.begin(), pngs.end(), [&](path const& p) {
                path imagepath = basedir/p;
                NSPasteboard* board = [NSPasteboard pasteboardWithUniqueName];
                boards.push_back(board);
                
                NSURL* url = [[NSURL alloc] initFileURLWithFilesystemPath:imagepath];
                NSImage* image = [[NSImage alloc] initWithContentsOfURL:url];
                images.insert({ imagepath, objc::object<NSImage>(image) });
                
                bool copied = objc::appkit::copy_to(board, image, url);
                CHECK(copied);
            });
            
            std::for_each(boards.begin(), boards.end(), [&](NSPasteboard* board) {
                bool can_paste_url = objc::appkit::can_paste<NSURL>(board);
                bool can_paste_image = objc::appkit::can_paste<NSImage>(board);
                CHECK(can_paste_url);
                CHECK(can_paste_image);
                
                NSURL* url = objc::appkit::paste<NSURL>(board);
                NSImage* boardimage = objc::appkit::paste<NSImage>(board);
                NSImage* mapimage = images.at([url filesystemPath]);
                NSData* boarddata = [boardimage TIFFRepresentation];
                NSData* mapdata = [mapimage TIFFRepresentation];
                
                CHECK(objc::to_bool([boarddata isEqualToData:mapdata]));
            });
            
            std::for_each(boards.begin(), boards.end(), [&](NSPasteboard* board) {
                [board releaseGlobally];
            });
            
        }
        
    }
    
    
    TEST_CASE("[appkit-copy-paste] Copy and paste JPEG image data",
              "[appkit-copy-paste-jpeg-image-data]")
    {
        path basedir(im::test::basedir);
        const std::vector<path> jpgs = basedir.list("*.jpg");
        
        @autoreleasepool {
            
            std::vector<NSPasteboard*> boards;
            std::unordered_map<path, objc::object<NSImage>> images;
            
            std::for_each(jpgs.begin(), jpgs.end(), [&](path const& p) {
                path imagepath = basedir/p;
                NSPasteboard* board = [NSPasteboard pasteboardWithUniqueName];
                boards.push_back(board);
                
                NSURL* url = [[NSURL alloc] initFileURLWithFilesystemPath:imagepath];
                NSImage* image = [[NSImage alloc] initWithContentsOfURL:url];
                images.insert({ imagepath, objc::object<NSImage>(image) });
                
                bool copied = objc::appkit::copy_to(board, image, url);
                CHECK(copied);
            });
            
            std::for_each(boards.begin(), boards.end(), [&](NSPasteboard* board) {
                bool can_paste_url = objc::appkit::can_paste<NSURL>(board);
                bool can_paste_image = objc::appkit::can_paste<NSImage>(board);
                CHECK(can_paste_url);
                CHECK(can_paste_image);
                
                NSURL* url = objc::appkit::paste<NSURL>(board);
                NSImage* boardimage = objc::appkit::paste<NSImage>(board);
                NSImage* mapimage = images.at([url filesystemPath]);
                NSData* boarddata = [boardimage TIFFRepresentation];
                NSData* mapdata = [mapimage TIFFRepresentation];
                
                CHECK(objc::to_bool([boarddata isEqualToData:mapdata]));
            });
            
            std::for_each(boards.begin(), boards.end(), [&](NSPasteboard* board) {
                [board releaseGlobally];
            });
            
        }
        
    }
    
}

