
#include <string>
#include <functional>
#include "fmt/format.h"

#include <libimread/libimread.hpp>
#include <libimread/errors.hh>
#include <libimread/ext/filesystem/path.h>
#include <libimread/ext/filesystem/execute.h>
#include <libimread/ext/filesystem/temporary.h>
#include <libimread/ext/filesystem/nowait.h>

#include <subjective-c/subjective-c.hpp>
#include <subjective-c/appkit.hh>
#import  <subjective-c/categories/NSURL+IM.hh>

#include "include/test_data.hpp"
#include "include/catch.hpp"

namespace {
    
    using filesystem::path;
    using filesystem::TemporaryDirectory;
    using filesystem::detail::nowait_t;
    
    path basedir(im::test::basedir);
    char const* wd = "/Users/fish/Dropbox/subjective-c/build/apps/impaste";
    
    std::string CLTRUN(std::string const& cmd) {
        return filesystem::detail::execute(path::join(wd, cmd), wd);
    }
    
    std::string copy_success_marker("Image successfully copied");
    std::string save_success_marker("Image successfully saved");
    
    TEST_CASE("[impaste-clt] Test impaste copy execution",
              "[impaste-clt-test-impaste-copy-execution]")
    {
        nowait_t nowait;
        const std::vector<path> jpgs = basedir.list("*.jpg");
        
        @autoreleasepool {
            std::for_each(jpgs.begin(), jpgs.end(), [&](path const& p) {
                path imagepath = basedir/p;
                std::string command = fmt::format("impaste {0} {1} {2}",
                                                  "-V", "-i", imagepath.str());
                std::string output = CLTRUN(command);
                
                WTF("OUTPUT:", output);
                // CHECK(output.find(copy_success_marker) != std::string::npos);
                
                NSURL* url = [[NSURL alloc] initFileURLWithFilesystemPath:imagepath];
                NSImage* image = [[NSImage alloc] initWithContentsOfURL:url];
                NSData* data = [image TIFFRepresentation];
                REQUIRE(objc::to_bool(objc::appkit::can_paste<NSImage>()));
                NSImage* boardimage = objc::appkit::paste<NSImage>();
                NSData* boarddata = [boardimage TIFFRepresentation];
                CHECK(objc::to_bool([boarddata isEqualToData:data]));
            });
        };
    }
    
    TEST_CASE("[impaste-clt] Test impaste paste execution",
              "[impaste-clt-test-impaste-paste-execution]")
    {
        nowait_t nowait;
        const std::vector<path> pngs = basedir.list("*.png");
        TemporaryDirectory td("test-impaste-paste");
        
        @autoreleasepool {
            std::for_each(pngs.begin(), pngs.end(), [&](path const& p) {
                path imagepath = basedir/p;
                NSURL* url = [[NSURL alloc] initFileURLWithFilesystemPath:imagepath];
                NSImage* image = [[NSImage alloc] initWithContentsOfURL:url];
                BOOL copied = objc::appkit::copy(image);
                REQUIRE(objc::to_bool(copied));
                path outputpath = td.dirpath/p;
                std::string command = fmt::format("impaste {0} {1} {2}",
                                                  "-V", "-o", outputpath.str());
                std::string output = CLTRUN(command);
                
                WTF("OUTPUT:", output);
                // CHECK(output.find(save_success_marker) != std::string::npos);
                
                NSData* data = [image TIFFRepresentation];
                NSURL* boardurl = [[NSURL alloc] initFileURLWithFilesystemPath:outputpath];
                NSImage* boardimage = [[NSImage alloc] initWithContentsOfURL:boardurl];
                NSData* boarddata = [boardimage TIFFRepresentation];
                CHECK(objc::to_bool([boarddata isEqualToData:data]));
            });
        };
    }
    
} /// namespace (anon.)