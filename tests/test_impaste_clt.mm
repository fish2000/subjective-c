
#include <string>
#include <functional>
#include "fmt/format.h"

#include <libimread/libimread.hpp>
#include <libimread/errors.hh>
#include <libimread/ext/filesystem/path.h>
#include <libimread/ext/filesystem/execute.h>
#include <libimread/ext/filesystem/temporary.h>
#include <subjective-c/subjective-c.hpp>
#include <subjective-c/appkit.hh>
#import  <subjective-c/categories/NSURL+IM.hh>

#include "include/test_data.hpp"
#include "include/catch.hpp"

namespace {
    
    using filesystem::path;
    using filesystem::TemporaryDirectory;
    
    path basedir(im::test::basedir);
    path bindir("/Users/fish/Dropbox/subjective-c/build/apps/impaste");
    
    std::string cltrun(std::string const& cmd) {
        char const* wd = "/Users/fish/Dropbox/subjective-c/build/apps/impaste";
        return filesystem::detail::execute(path::join(wd, cmd), wd);
    }
    
    std::string copy_success_marker("Image successfully copied");
    std::string save_success_marker("Image successfully saved");
    
    TEST_CASE("[impaste-clt] Test impaste copy execution",
              "[impaste-clt-test-impaste-copy-execution]")
    {
        const std::vector<path> jpgs = basedir.list("*.jpg");
        
        @autoreleasepool {
            std::for_each(jpgs.begin(), jpgs.end(), [&](path const& p) {
                path imagepath = basedir/p;
                std::string command = fmt::format("impaste {0} {1} {2}",
                                                  "-V", "-i",
                                                  imagepath.make_absolute().str());
                std::string output = cltrun(command);
                CHECK(output.find(copy_success_marker) != std::string::npos);
                NSURL* url = [[NSURL alloc] initFileURLWithFilesystemPath:imagepath];
                NSImage* image = [[NSImage alloc] initWithContentsOfURL:url];
                NSData* data = [image TIFFRepresentation];
                NSImage* boardimage = objc::appkit::paste<NSImage>();
                NSData* boarddata = [boardimage TIFFRepresentation];
                CHECK(objc::to_bool([boarddata isEqualToData:data]));
            });
        };
    }
    
    TEST_CASE("[impaste-clt] Test impaste paste execution",
              "[impaste-clt-test-impaste-paste-execution]")
    {
        const std::vector<path> pngs = basedir.list("*.png");
        TemporaryDirectory td("test-impaste-paste");
        
        @autoreleasepool {
            std::for_each(pngs.begin(), pngs.end(), [&](path const& p) {
                path imagepath = basedir/p;
                NSURL* url = [[NSURL alloc] initFileURLWithFilesystemPath:imagepath];
                NSImage* image = [[NSImage alloc] initWithContentsOfURL:url];
                BOOL copied = objc::appkit::copy(image);
                CHECK(objc::to_bool(copied));
                path outputpath = td.dirpath/p+".png";
                std::string command = fmt::format("impaste {0} {1} {2}",
                                                  "-V", "-o",
                                                  outputpath.make_absolute().str());
                std::string output = cltrun(command);
                CHECK(output.find(save_success_marker) != std::string::npos);
                NSData* data = [image TIFFRepresentation];
                NSURL* boardurl = [[NSURL alloc] initFileURLWithFilesystemPath:outputpath];
                NSImage* boardimage = [[NSImage alloc] initWithContentsOfURL:boardurl];
                NSData* boarddata = [boardimage TIFFRepresentation];
                CHECK(objc::to_bool([boarddata isEqualToData:data]));
            });
        };
    }
    
} /// namespace (anon.)