
#include <string>
#include <functional>
#include "fmt/format.h"

#include <libimread/libimread.hpp>
#include <libimread/errors.hh>
#include <libimread/ext/filesystem/path.h>
#include <libimread/ext/filesystem/execute.h>
#include <subjective-c/subjective-c.hpp>
#include <subjective-c/appkit.hh>
#import  <subjective-c/categories/NSURL+IM.hh>

#include "include/test_data.hpp"
#include "include/catch.hpp"

namespace {
    
    using filesystem::path;
    // using filesystem::detail::execute;
    // using namespace std::placeholders;
    
    path basedir(im::test::basedir);
    // path bindir = path::absolute("./../build/apps/impaste");
    path bindir("/Users/fish/Dropbox/subjective-c/build/apps/impaste");
    // const char* cbindir = bindir.c_str();s
    // std::string sbindir(bindir.str());
    // auto cltrun = std::bind(filesystem::detail::execute, _1, std::cref(cbindir));
    std::string cltrun(std::string const& cmd) {
        char const* wd = "/Users/fish/Dropbox/subjective-c/build/apps/impaste";
        // char const* wd = bindir.c_str();
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
    
} /// namespace (anon.)