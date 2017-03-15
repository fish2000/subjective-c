/// Copyright 2012-2017 Alexander Bohn <fish2000@gmail.com>
/// License: MIT (see COPYING.MIT file)

#include "impaste.hh"
#include <subjective-c/appkit.hh>

#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <map>

#include <libimread/ext/filesystem/path.h>
#import  <subjective-c/categories/NSString+STL.hh>
#import  <subjective-c/categories/NSURL+IM.hh>
#include "docopt.h"

/// return-value as static global (ugh I know I know)
std::atomic<int> return_value{ EXIT_SUCCESS };

/// verbosity-level as static global
std::atomic<int> verbosity{ 0 };

/// string vector
using stringvec_t = std::vector<std::string>;

/// App delegate
@implementation AXAppDelegate
- (void) applicationWillTerminate:(NSApplication*)application {
    
    if (verbosity.load() > 0) {
        std::cout << "[impaste] Exiting with status: "
                  << return_value.load()
                  << std::endl << std::endl;
    }
    
    std::exit(return_value.load());
}
@end

/// Base thread implementation
@implementation AXThread
@synthesize options;
- (instancetype) initWithOptions:(NSDictionary*)optionsDict {
    self = [self init];
    self.options = [NSDictionary dictionaryWithDictionary:optionsDict];
    return self;
}
@end

/// NSThread declarations and definitions,
/// one(ish) for each CLI option

@implementation AXCheckThread : AXThread
- (void) main {
    
    if (verbosity.load() > 0) {
        std::cout << "[impaste] Checking default NSPasteboard for images ..."
                  << std::endl;
    }
    
    bool ok = objc::to_bool(
              objc::appkit::can_paste<NSImage>());
    
    if (ok) {
        
        std::cout << "[impaste] Pasteboard contains useable image data [go nuts!]"
                  << std::endl;
        AXTHREADEXIT(EXIT_SUCCESS);
        
    } else { /// THIS IS NOT TOWARD
        
        std::cout << "[impaste] No useable image data found [sorry dogg]"
                  << std::endl;
        AXTHREADEXIT(EXIT_FAILURE);
        
    }
    
}
@end

@implementation AXDryRunThread : AXThread
- (void) main {
    
    std::cerr << "[impaste] Dry run not implemented yet -- exiting..."
              << std::endl;
    AXTHREADEXIT(EXIT_FAILURE);
    
}
@end

@implementation AXImageCopyAndSaveThread : AXThread
- (void) main {
    
    if (self.options[@"input"]) {
        
        NSString* inpathstring = self.options[@"input"];
        NSURL* inpathurl = [NSURL fileURLWithPath:inpathstring.stringByExpandingTildeInPath];
        filesystem::path inabspath = [inpathurl filesystemPath].make_absolute();
        
        if (!inabspath.is_readable()) {
            std::cerr << "[impaste][error] No such readable image file exists: "
                      << [inpathstring STLString]       << std::endl
                      << "\t(" << inabspath << ") ..."  << std::endl;
            AXTHREADEXIT(EXIT_FAILURE);
        }
        
        if (!objc::to_bool([inpathurl isImage])) {
            std::cerr << "[impaste][error] Can't determine input format from filename: "
                      << inabspath.basename()
                      << std::endl;
            AXTHREADEXIT(EXIT_FAILURE);
        }
        
        if (verbosity.load() > 0) {
            std::cout << "[impaste] Copying "
                      << [[inpathurl.pathExtension uppercaseString] STLString]
                      << " image to pasteboard from "
                      << inabspath.basename()
                      << " (" << inabspath << ") ..."  << std::endl;
        }
        
        NSImage* copyTarget = [[NSImage alloc] initWithContentsOfURL:inpathurl];
        bool copied = objc::to_bool(
                      objc::appkit::copy(copyTarget));
        
        if (copied) {
            std::cout << "[impaste] Image successfully copied! ("
                      << std::round(inabspath.filesize() / 1024) << "kbytes)"
                      << std::endl;
            if (self.options[@"output"] == nil) {
                AXTHREADEXIT(EXIT_SUCCESS);
            }
        } else {
            std::cerr << "[impaste][error] Failure when reading image data"
                      << std::endl;
            if (self.options[@"output"] == nil) {
                AXTHREADEXIT(EXIT_FAILURE);
            }
        }
        
    }
    
    if (self.options[@"output"]) {
        
        NSString* outpathstring = self.options[@"output"];
        NSURL* outpathurl = [NSURL fileURLWithPath:outpathstring.stringByExpandingTildeInPath];
        filesystem::path outabspath = [outpathurl filesystemPath].make_absolute();
        
        if (outabspath.exists()) {
            std::cerr << "[impaste][error] File already exists at path: "
                      << [outpathstring STLString]      << std::endl
                      << "\t(" << outabspath << ") ..." << std::endl;
            AXTHREADEXIT(EXIT_FAILURE);
        }
        
        if (!objc::to_bool([outpathurl isImage])) {
            std::cerr << "[impaste][error] Can't determine output format from filename: "
                      << outabspath.basename()
                      << std::endl;
            AXTHREADEXIT(EXIT_FAILURE);
        }
        
        NSImage* pasted = objc::appkit::paste<NSImage>();
        NSBitmapImageRep* bitmap = [[NSBitmapImageRep alloc] initWithData:[pasted TIFFRepresentation]];
        NSData* data = [bitmap representationUsingType:[outpathurl imageFileType]
                                            properties:@{}];
        
        if (verbosity.load() > 0) {
            std::cout << "[impaste] Saving "
                      << [[outpathurl.pathExtension uppercaseString] STLString]
                      << " image from pasteboard to "
                      << outabspath.basename()
                      << " (" << outabspath << ") ..." << std::endl;
        }
        
        BOOL saved = [data writeToURL:outpathurl atomically:YES];
        
        if (objc::to_bool(saved)) {
            std::cout << "[impaste] Image successfully saved! ("
                      << std::round(outabspath.filesize() / 1024) << "kbytes)"
                      << std::endl;
            AXTHREADEXIT(EXIT_SUCCESS);
        } else {
            std::cerr << "[impaste][error] Failure when writing image data"
                      << std::endl;
            AXTHREADEXIT(EXIT_FAILURE);
        }
        
    }

}
@end

/// The docopt help string defines the options available:
const char USAGE[] = R"([impaste] Paste image data to imgur.com or to a file

    Usage:
        impaste         [-c         | --check       ]
                        [-d         | --dry-run     ]
                        [-V         | --verbose     ]
                        [-i FILE    | --input=FILE  ]
                        [-o FILE    | --output=FILE ]
        
        impaste         -h | --help | -v | --version
    
    Options:
        -c --check                  Check and report on the pasteboard contents.
        -d --dry-run                Don't actually do anything, but pretend.
        -V --verbose                Print more information.
        -i FILE --input=FILE        Copy an image to the pasteboard.
        -o FILE --output=FILE       Save pasteboard image to a file.
        -h --help                   Show this help screen.
        -v --version                Show version.
)";

const std::string VERSION = "[impaste]{ subjective-c " + objc::config::version + " }";

int main(int argc, const char** argv) {
    using value_t = docopt::value;
    using optmap_t = std::map<std::string, value_t>;
    using optpair_t = std::pair<std::string, value_t>;
    value_t truth(1);
    value_t empty(NULL);
    bool debug{ IMPASTE_DEBUG };
    optmap_t args;
    optmap_t raw_args = docopt::docopt(USAGE, { argv + 1, argv + argc },
                                       true, /// show help
                                       VERSION);
    
    if (debug) {
        std::cerr << std::endl
                  << "[impaste] RAW ARGS:" << std::endl;
        for (optpair_t const& arg : raw_args) {
            std::cerr << "\t" << arg.first  << " --> "
                              << arg.second << std::endl;
        }
    }
    
    /// filter out all docopt parse artifacts,
    /// leaving only things beginning with "--"
    std::copy_if(raw_args.begin(),
                 raw_args.end(),
                 std::inserter(args, args.begin()),
              [](optpair_t const& p) { return p.first.substr(0, 1) == "-"; });
    
    if (debug) {
        std::cerr << std::endl
                  << "[impaste] FILTERED ARGS:" << std::endl;
        for (optpair_t const& arg : args) {
            std::cerr << "\t" << arg.first  << " --> "
                              << arg.second << std::endl;
        }
        std::cerr << std::endl;
    }
    
    /// print the value for the truthy option flag
    for (optpair_t const& arg : args) {
        if (arg.first == "--verbose" || arg.first == "-V") {
            verbosity.store(arg.second.asLong() + (int)debug);
            if (verbosity.load() > 0) {
                std::cout << "[impaste] VERBOSITY: " << verbosity.load() << std::endl;
            }
        }
    }
    
    NSMutableDictionary<NSString*, NSString*>* options = [[NSMutableDictionary alloc] init];
    
    for (optpair_t const& arg : args) {
        if (arg.second == truth) {
            if (arg.first == "--check" || arg.first == "-c") {
                /* DO CHECK */
                objc::run_thread<AXCheckThread>(options);
                break;
            } else if (arg.first == "--dry-run" || arg.first == "-d") {
                /* DO DRY RUN */
                objc::run_thread<AXDryRunThread>(options);
                break;
            }
        }
        if (arg.second != empty) {
            std::string path{ NULL_STR };
            if (arg.second.isString()) {
                path = arg.second.asString();
            } else if (arg.second.isStringList()) {
                stringvec_t stringvec = arg.second.asStringList();
                if (stringvec.size() > 0) {
                    path = stringvec.at(0);
                }
            }
            if (path != NULL_STR) {
                if (arg.first == "--input" || arg.first == "-i") {
                    /* DO FILE INPUT */
                    [options setObject:[NSString stringWithSTLString:path]
                                forKey:@"input"];
                }
                if (arg.first == "--output" || arg.first == "-o") {
                    /* DO FILE OUTPUT */
                    [options setObject:[NSString stringWithSTLString:path]
                                forKey:@"output"];
                }
            }
        }
    }
    
    if (options[@"input"] || options[@"output"]) {
        objc::run_thread<AXImageCopyAndSaveThread>(options);
    }
    
    /// doesn't get called from threads
    std::exit(return_value.load());

}
